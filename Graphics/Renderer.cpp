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
        m_instancedSpriteShader = Shader::createSrc(shader::instancedSpriteVert, shader::spriteFrag);
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

        m_instancedSpriteVBO = VertexBuffer::createZeroed(sizeof(Mat4), 20, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);
        m_instancedSpriteOffsets = VertexBuffer::createZeroed(sizeof(Vec4f), 20, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);

        m_vao.setLayout(m_instancedSpriteOffsets, 4, false, sizeof(Vec4f), 0);

        for (int i = 0; i < 4; i++)
            m_vao.setLayout(m_instancedSpriteVBO, 4, false, sizeof(Mat4), (void*)(i * sizeof(Vec4f)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);

        unbindVAO();
    }

    {
        m_userVAO = VertexArray::create();
        m_userVAO.bind();

        m_userBuffer = VertexBuffer::createZeroed(sizeof(Vertex2D), 20, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);

        m_userVAO.setLayout(m_userBuffer, 3, false, sizeof(Vertex2D), 0);
        m_userVAO.setLayout(m_userBuffer, 2, false, sizeof(Vertex2D), (void*)GET_MEM_OFFSET(Vertex2D, texCoord));
        m_userVAO.setLayout(m_userBuffer, 4, false, sizeof(Vertex2D), (void*)GET_MEM_OFFSET(Vertex2D, color));

        unbindVAO();
    }

}

pure::Renderer::~Renderer()
{
    m_vao.free();
    m_vertBuffer.free();
    m_instancedSpriteVBO.free();
    m_instancedSpriteShader.free();
    m_colorShader.free();
    m_spriteShader.free();
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
    Mat4 model = translate(makeMat4(), Vec3f(r.x, r.y, 0.f));
    model = translate(model, Vec3f(r.w * .5f, r.h * .5f, 0.f));
    model = rotate(model, radians(rotation), Vec3f(0.f, 0.f, 1.f));
    model = translate(model, Vec3f(r.w * -.5f, r.h * -.5f, 0.f));
    model = scale(model, Vec3f(r.w, r.h, 1.f));


    m_colorShader.setUniform("u_color", color);
    m_colorShader.setUniform("u_matrix", m_projection * cam.view() * model);
    drawArrays(DrawPrimitive::TRIANGLES, 0, m_vertBuffer.count);
}

void pure::Renderer::drawTexture(const Texture& tex, Vec3f pos, Vec2f size, float rotation) const
{
    drawTexture(tex, pos, size, m_colorShader, rotation);
}

void Renderer::drawTexture(const Texture &tex, Vec3f pos, Vec2f size, Shader shader, float rotation) const
{
    m_vao.bind();
    shader.bind();
    tex.bind();

    Mat4 model = translate(makeMat4(), pos);
    model = translate(model, Vec3f(size.x * .5f, size.y * .5f, 0.f));
    model = rotate(model, radians(rotation), Vec3f(0.f, 0.f, 1.f));
    model = translate(model, Vec3f(size.x * -.5f, size.y * -.5f, 0.f));
    model = scale(model, Vec3f(size.x, size.y, 1.f));

    m_spriteShader.setUniform("u_matrix", m_projection * cam.view() *  model);

    drawArrays(DrawPrimitive::TRIANGLES, 0, m_vertBuffer.count);
}

void pure::Renderer::drawSprite(Sprite & sprite) const
{
    drawSprite(sprite, m_spriteShader);
}

void Renderer::drawSprite(Sprite &sprite, Shader shader) const
{
    m_vao.bind();
    m_spriteShader.bind();
    sprite.texture()->bind();

    m_spriteShader.setUniform("u_matrix", m_projection * cam.view() * sprite.modelMatrix());

    {
        const Rectui& texRect = sprite.textureRect;

        m_spriteShader.setUniform("u_textureOffsets", Vec4f(
                float(texRect.x), float(texRect.y), float(texRect.w), float(texRect.h)
        ));
    }

    drawArrays(DrawPrimitive::TRIANGLES, 0, m_vertBuffer.count);
}

// TODO: Test to make sure this is actually faster lol
void pure::Renderer::drawSpritesInstanced(Sprite * sprites, size_t count)
{
    drawSpritesInstanced(sprites,count, m_instancedSpriteShader);
}

void Renderer::drawSpritesInstanced(Sprite *sprites, size_t count, Shader shader)
{
    m_vao.bind();
    m_instancedSpriteShader.bind();
    sprites[0].texture()->bind();

    std::vector<Mat4> transforms;
    transforms.reserve(count);
    std::vector<Vec4f> texOffsets;
    texOffsets.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        Sprite& sprite = sprites[i];
        transforms.push_back(m_projection *  cam.view() * sprite.modelMatrix());

        {
            const Rectui& texRect = sprite.textureRect;
            texOffsets.emplace_back(float(texRect.x), float(texRect.y), float(texRect.w), float(texRect.h));
        }
    }

    if (transforms.size() >= m_instancedSpriteVBO.capacity)
        m_instancedSpriteVBO.alloc(&transforms[0], transforms.size(), DrawUsage::DYNAMIC_DRAW);
    else
        m_instancedSpriteVBO.writeBuffer(&transforms[0], transforms.size(), 0);

    if (texOffsets.size() >= m_instancedSpriteOffsets.capacity)
        m_instancedSpriteOffsets.alloc(&texOffsets[0], texOffsets.size(), DrawUsage::DYNAMIC_DRAW);
    else
        m_instancedSpriteOffsets.writeBuffer(&texOffsets[0], texOffsets.size(), 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertBuffer.count, count);
}

void pure::Renderer::setViewport(const Rectf& vp)
{
    m_projection = orthographic(vp.x, vp.w, vp.h, vp.y, CLIP_NEAR, CLIP_FAR);
    m_viewport = vp;
}

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
    m_userVAO.bind();
    shader.bind();

    if (vertCount >= m_userBuffer.capacity)
        m_userBuffer.alloc(verts, vertCount, DrawUsage::DYNAMIC_DRAW);
    else
        m_userBuffer.writeBuffer(verts, vertCount, 0);

    m_basicShader.setUniform("u_matrix", m_projection * cam.view());

    drawArrays(primtype, 0, m_userBuffer.count);
}

constexpr const char* FRAG_TEMPLATE_VARS = "#version 330\n"
        "out vec4 FragColor;\n"

        "in vec2 TexCoord;\n"
        "in vec4 Color;\n"
        "in vec3 FragPos;\n"

        "uniform sampler2D u_texture;\n";

constexpr const char* FRAG_TEMPLATE_MAIN = "\n"
        "void main()\n"
        "{\n"
        "	FragColor = effect(Color, u_texture, TexCoords, FragPos);\n"
        "}";

Shader Renderer::createShader(const char *effectSrc, bool isInstanced)
{
    const char* vert = isInstanced ? shader::instancedSpriteVert : shader::vert;

    std::string frag = FRAG_TEMPLATE_VARS;
    frag += effectSrc;
    frag += FRAG_TEMPLATE_MAIN;

    return Shader::createSrc(vert, frag.c_str());

}


