//
// Created by matt on 9/3/18.
//
#include "Renderer.h"
#include "Math/MatrixTransform.h"
#include "Math/Convert.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Window/Window.h"
#include "Private/Shaders.h"
#include "Private/Util.h"
#include <string>
#include <vector>
#include "glad.h"

using namespace pure;

#define GET_MEM_OFFSET(t, member) (&(((t*)0)->member))

static constexpr float CLIP_NEAR = -50.f;
static constexpr float CLIP_FAR = 50.f;

struct InstancedVertex2D
{
    static constexpr size_t COUNT = 12;

    union
    {
        struct
        {
            struct { float x, y, z, w; } xTransform;
            struct { float x, y, z, w; } yTransform;
            struct { float x, y, z, w;} zTransform;
        };

        float buffer[COUNT];
    };
};

pure::Renderer::Renderer(const Window& window) : cam({})
{

    {
        cam.position.z = -5.f;
        cam.lookDir.z = -1.f;
    }

    {

        {
            const auto w = static_cast<float>(window.width());
            const auto h = static_cast<float>(window.height());
            const Rectf viewport = { 0.f, 0.f, w, h };
            setViewport({ 0.f, 0.f, w, h });
        }

        m_spriteShader = Shader::createSrc(shader::vert, shader::spriteFrag);
        m_colorShader = Shader::createSrc(shader::vert, shader::colorFrag);
        m_instancedShader = Shader::createSrc(shader::instancedSpriteVert, shader::spriteFrag);
        m_basicShader = Shader::createSrc(shader::vert, shader::frag);

        m_spriteShader.bind();
        m_spriteShader.setUniform("u_texture", 0);
    }

    {

        constexpr Vertex2D verts[] = {
                { 0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },
                { 1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
                { 0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },

                { 0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },
                { 1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },
                { 1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        };

        m_vao = VertexArray::create();
        m_vao.bind();

        m_vertBuffer = VertexBuffer::create(verts, ARRAY_COUNT(verts), DrawUsage::STATIC_DRAW);

        m_vao.setLayout(m_vertBuffer, 3, false, sizeof(Vertex2D), 0);
        m_vao.setLayout(m_vertBuffer, 2, false, sizeof(Vertex2D), (void*)GET_MEM_OFFSET(Vertex2D, texCoord));
        m_vao.setLayout(m_vertBuffer, 4, false, sizeof(Vertex2D), (void*)GET_MEM_OFFSET(Vertex2D, color));

//        m_instancedMatBuffer = VertexBuffer::createZeroed(sizeof(Mat4), 20, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);
//        m_instancedOffsetsBuffer = VertexBuffer::createZeroed(sizeof(Vec4f), 20, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);
//
//        m_vao.setLayout(m_instancedOffsetsBuffer, 4, false, sizeof(Vec4f), 0);
//
//        for (int i = 0; i < 4; i++)
//            m_vao.setLayout(m_instancedMatBuffer, 4, false, sizeof(Mat4), (void*)(i * sizeof(Vec4f)));
//
//        glVertexAttribDivisor(3, 1);
//        glVertexAttribDivisor(4, 1);
//        glVertexAttribDivisor(5, 1);
//        glVertexAttribDivisor(6, 1);
//        glVertexAttribDivisor(7, 1);

        constexpr int NUM_MAT4 = 2;

        m_instancedMatBuffer = VertexBuffer::createZeroed(sizeof(Mat4) * NUM_MAT4, 20, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);
        m_instancedOffsetsBuffer = VertexBuffer::createZeroed(sizeof(Vec4f), 20, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);

        m_vao.setLayout(m_instancedOffsetsBuffer, 4, false, sizeof(Vec4f), 0);

        for (int i = 0; i < 4 * NUM_MAT4; i++)
            m_vao.setLayout(m_instancedMatBuffer, 4, false, sizeof(Mat4) * NUM_MAT4, (void*)(i * sizeof(Vec4f)));

        for (int i = 3; i <= 11; i++)
            glVertexAttribDivisor(i, 1);

        unbindVAO();
    }

    {
        m_primitiveVAO = VertexArray::create();
        m_primitiveVAO.bind();

        m_primitiveBuffer = VertexBuffer::createZeroed(sizeof(Vertex2D), 20, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);

        m_primitiveVAO.setLayout(m_primitiveBuffer, 3, false, sizeof(Vertex2D), 0);
        m_primitiveVAO.setLayout(m_primitiveBuffer, 2, false, sizeof(Vertex2D), (void*)GET_MEM_OFFSET(Vertex2D, texCoord));
        m_primitiveVAO.setLayout(m_primitiveBuffer, 4, false, sizeof(Vertex2D), (void*)GET_MEM_OFFSET(Vertex2D, color));

        unbindVAO();
    }

}

pure::Renderer::~Renderer()
{
    m_vao.free();
    m_vertBuffer.free();
    m_instancedMatBuffer.free();
    m_instancedShader.free();
    m_colorShader.free();
    m_spriteShader.free();
    m_basicShader.free();

    m_primitiveVAO.free();
    m_primitiveBuffer.free();
}

const Rectf & pure::Renderer::viewport() const { return m_viewport; }

void pure::Renderer::drawRect(const Rectf& r, const Vec4<float>& color, float rotation) const
{
    drawRect(r, color, m_colorShader, rotation);
}

void Renderer::drawRect(const Rectf &r, const Vec4<float> &color, Shader shader, float rotation) const
{
    m_vao.bind();
    shader.bind();
    Mat4 model = translate(makeMat4(), Vec3f(r.x, r.y, 20.f));
    model = translate(model, Vec3f(r.w * .5f, r.h * .5f, 0.f));
    model = rotate(model, radians(rotation), Vec3f(0.f, 0.f, 1.f));
    model = translate(model, Vec3f(r.w * -.5f, r.h * -.5f, 0.f));
    model = scale(model, Vec3f(r.w, r.h, 1.f));


    shader.setUniform("u_color", color);

    shader.setUniform("u_modelMatrix", model);
    shader.setUniform("u_matrixMVP", m_projection * cam.view() * model);
    drawArrays(DrawPrimitive::TRIANGLES, 0, m_vertBuffer.count);
}

void pure::Renderer::drawTexture(const Texture& tex, Vec3f pos, Vec2f size, float rotation, const Rectui* texRect) const
{
    drawTexture(tex, pos, size, m_spriteShader, rotation, texRect);
}

void Renderer::drawTexture(const Texture &tex, Vec3f pos, Vec2f size, Shader shader, float rotation, const Rectui* texRect) const
{
    Mat4 model = translate(makeMat4(), pos);
    model = translate(model, Vec3f(size.x * .5f, size.y * .5f, 0.f));
    model = rotate(model, radians(rotation), Vec3f(0.f, 0.f, 1.f));
    model = translate(model, Vec3f(size.x * -.5f, size.y * -.5f, 0.f));
    model = scale(model, Vec3f(size.x, size.y, 1.f));

    drawTexture(tex, model, shader, texRect);
}

void pure::Renderer::drawSprite(Sprite & sprite) const
{
    drawSprite(sprite, m_spriteShader);
}

void Renderer::drawTexture(const Texture &tex, const Mat4 &transform, const Rectui *texRect) const
{
    drawTexture(tex, transform, m_spriteShader, texRect);
}

void Renderer::drawTexture(const Texture &tex, const Mat4 &transform, Shader shader, const Rectui *texRect) const
{
    m_vao.bind();
    shader.bind();
    tex.bind();

    shader.setUniform("u_modelMatrix", transform);
    shader.setUniform("u_matrixMVP", m_projection * cam.view() *  transform);

    Rectui textureRect;
    if (!texRect)
        textureRect = { 0, 0, uint32_t(tex.size.x), uint32_t(tex.size.y) };
    else
        textureRect = *texRect;

    shader.setUniform("u_textureOffsets", Vec4f(
            float(textureRect.x), float(textureRect.y), float(textureRect.w), float(textureRect.h)
    ));

    drawArrays(DrawPrimitive::TRIANGLES, 0, m_vertBuffer.count);
}


void Renderer::drawSprite(Sprite &sprite, Shader shader) const
{
    drawTexture(*sprite.texture(), sprite.modelMatrix(), shader, &sprite.textureRect);
}

// TODO: Test to make sure this is actually faster lol
void pure::Renderer::drawSpritesInstanced(Sprite * sprites, size_t count)
{
    drawSpritesInstanced(sprites,count, m_instancedShader);
}

void Renderer::drawSpritesInstanced(Sprite *sprites, size_t count, Shader shader)
{

    m_vao.bind();
    shader.bind();
    sprites[0].texture()->bind();

    std::vector<Mat4> transforms;
    transforms.reserve(count);
    std::vector<Vec4f> texOffsets;
    texOffsets.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        Sprite& sprite = sprites[i];
        transforms.push_back(m_projection *  cam.view() * sprite.modelMatrix());
        transforms.push_back(sprite.modelMatrix());

        {
            const Rectui& texRect = sprite.textureRect;
            texOffsets.emplace_back(float(texRect.x), float(texRect.y), float(texRect.w), float(texRect.h));
        }
    }

    if (transforms.size() >= m_instancedMatBuffer.capacity)
        m_instancedMatBuffer.alloc(&transforms[0], transforms.size(), DrawUsage::DYNAMIC_DRAW);
    else
        m_instancedMatBuffer.writeBuffer(&transforms[0], transforms.size(), 0);

    if (texOffsets.size() >= m_instancedOffsetsBuffer.capacity)
        m_instancedOffsetsBuffer.alloc(&texOffsets[0], texOffsets.size(), DrawUsage::DYNAMIC_DRAW);
    else
        m_instancedOffsetsBuffer.writeBuffer(&texOffsets[0], texOffsets.size(), 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertBuffer.count, count);
}

void pure::Renderer::setViewport(const Rectf& vp)
{
    m_projection = orthographic(vp.x, vp.w, vp.h, vp.y, CLIP_NEAR, CLIP_FAR);
    m_viewport = vp;
}

// TODO: This doesn't kind of zooms from the top left corner, make this zoom from center.
void pure::Renderer::zoom(float offset)
{
    m_viewport.w += offset;
    m_viewport.h += offset;
    m_projection = orthographic(m_viewport.x,
                                m_viewport.w, m_viewport.h,
                                m_viewport.y, CLIP_NEAR, CLIP_FAR
    );

}

void Renderer::drawPrimitive(DrawPrimitive primtype, const Vertex2D *verts, size_t vertCount)
{
    drawPrimitive(primtype, verts, vertCount, m_basicShader);
}

void Renderer::drawPrimitive(DrawPrimitive primtype, const Vertex2D *verts, size_t vertCount, Shader shader)
{
    m_primitiveVAO.bind();
    shader.bind();

    if (vertCount >= m_primitiveBuffer.capacity)
        m_primitiveBuffer.alloc(verts, vertCount, DrawUsage::DYNAMIC_DRAW);
    else
        m_primitiveBuffer.writeBuffer(verts, vertCount, 0);

    m_basicShader.setUniform("u_matrixMVP", m_projection * cam.view());
    m_basicShader.setUniform("u_modelMatrix", makeMat4());

    drawArrays(primtype, 0, m_primitiveBuffer.count);
}

// TODO: I don't like how we handle paramters for instanced drawing, it would cause user to allocate memory and/or separate existing
// memory to conform to the api. Maybe use some kind of polymorphism instead.
void Renderer::drawTextureInstanced(const Texture& tex, const Mat4* transforms, const Rectui* texRects, size_t count)
{
    drawTextureInstanced(tex, transforms, texRects, count, m_instancedShader);
}

void Renderer::drawTextureInstanced(const Texture& tex, const Mat4* transforms, const Rectui* texRects, size_t count, Shader shader)
{
    m_vao.bind();
    shader.bind();
    tex.bind();

    std::vector<Mat4> transformData;
    transformData.reserve(count);
    std::vector<Vec4f> texOffsets;
    texOffsets.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        const Mat4& model = transforms[i];
        transformData.push_back(m_projection *  cam.view() * model);
        transformData.push_back(model);

        {
            const Rectui& texRect = texRects[i];
            texOffsets.emplace_back(float(texRect.x), float(texRect.y), float(texRect.w), float(texRect.h));
        }
    }

    if (transformData.size() >= m_instancedMatBuffer.capacity)
        m_instancedMatBuffer.alloc(&transformData[0], transformData.size(), DrawUsage::DYNAMIC_DRAW);
    else
        m_instancedMatBuffer.writeBuffer(&transformData[0], transformData.size(), 0);

    if (texOffsets.size() >= m_instancedOffsetsBuffer.capacity)
        m_instancedOffsetsBuffer.alloc(&texOffsets[0], texOffsets.size(), DrawUsage::DYNAMIC_DRAW);
    else
        m_instancedOffsetsBuffer.writeBuffer(&texOffsets[0], texOffsets.size(), 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertBuffer.count, count);
}
