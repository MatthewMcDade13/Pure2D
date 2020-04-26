project('Pure2D', 'cpp', 'c', default_options : ['c_std=c11', 'cpp_std=c++17'])

src = [
    'Init.cpp',
    'Audio/Audio.cpp',
    'Audio/AudioSource.cpp',
    'Audio/Listener.cpp',
    'Audio/Music.cpp',
    'Audio/SoundBuffer.cpp',
    'Engine/Game.cpp',
    'External/glad.cpp',
    'External/stb_image.cpp',
    'External/stb_truetype.cpp',
    'Graphics/Buffers.cpp',
    'Graphics/Camera.cpp',
    'Graphics/Font.cpp',
    'Graphics/Mesh.cpp',
    'Graphics/Quad.cpp',
    'Graphics/Renderer.cpp',
    'Graphics/RenderTexture.cpp',
    'Graphics/Shader.cpp',
    'Graphics/SpriteBatch.cpp',
    'Graphics/Texture.cpp',
    'Graphics/Transform.cpp',
    'Graphics/Vertex.cpp',
    'Math/Manip.cpp',
    'Math/Mat4.cpp',
    'Math/MatrixTransform.cpp',
    'Private/Convert.cpp',
    'Private/ErrorHandling.cpp',
    'Private/FileIO.cpp',
    'Private/GlContext.cpp',
    'System/Time.cpp',
    'Window/Keyboard.cpp',
    'Window/Window.cpp'
]

dep_dl = meson.get_compiler('cpp').find_library('dl')
dep_sdl2 = meson.get_compiler('cpp').find_library('SDL2') #dependency('sdl2')
dep_sdl2mixer = meson.get_compiler('cpp').find_library('SDL2_mixer') #dependency('sdl2_mixer')
dep_openal = dependency('openal')
dep_freetype = dependency('freetype2')
dep_glm = dependency('glm')
dep_glfw = dependency('glfw3')
dep_all = [dep_sdl2, dep_sdl2mixer, dep_openal, dep_freetype, dep_glm, dep_glfw, dep_dl]

# dir_self = include_directories('.')
# dir_engine = include_directories('Engine')
# dir_external = include_directories('External')  
# dir_graphics = include_directories('Graphics')
# dir_math = include_directories('Math')
# dir_private = include_directories('Private')
# dir_system = include_directories('System')
# dir_window = include_directories('Window')
# dir_all = [dir_self, dir_engine, dir_external, dir_graphics, dir_math, dir_private, dir_system, dir_window]

install_headers([
    'Audio.h', 'Define.h', 'Graphics.h', 'Init.h', 'Pure2D.h', 'PureMath.h', 'System.h', 'Window.h'
], subdir: 'Pure2D')

install_headers([
    'Audio/Audio.h', 'Audio/AudioSource.h', 'Audio/Listener.h', 
    'Audio/Music.h', 'Audio/SoundBuffer.h'
], subdir: 'Pure2D/Audio')

install_headers(['Engine/Game.h'], subdir: 'Engine')

install_headers([
    'Graphics/Buffers.h', 'Graphics/Camera.h', 'Graphics/Color.h',
    'Graphics/Font.h', 'Graphics/Mesh.h', 'Graphics/Quad.h', 'Graphics/Renderable.h',
    'Graphics/Renderer.h', 'Graphics/RenderTexture.h', 'Graphics/Shader.h', 'Graphics/SpriteBatch.h',
    'Graphics/Texture.h', 'Graphics/Transform.h', 'Graphics/Vertex.h'
], subdir: 'Pure2D/Graphics')

install_headers([
    'Math/Constants.h', 'Math/Convert.h', 'Math/Manip.h', 'Math/Mat4.h',
    'Math/MatrixTransform.h', 'Math/Rect.h', 'Math/Vec2.h', 'Math/Vec3.h', 'Math/Vec4.h'
], subdir: 'Pure2D/Math')

install_headers([
    'System/DataType.h', 'System/Defer.h', 'System/Logging.h', 'System/NonCopyable.h',
    'System/Time.h', 'System/Util.h'
], subdir: 'Pure2D/System')  

install_headers([
    'Window/Keyboard.h', 'Window/Window.h', 'Window/WindowEvent.h'
], subdir: 'Pure2D/Window')

dir_rel = include_directories('../')
pure2dlib = library('pure2d', src, install: true, dependencies: dep_all, include_directories: [dir_rel])

executable('sample_quad', 'Examples/RenderQuad.cpp', link_with: pure2dlib, include_directories: [dir_rel])

