import shutil
import os.path

def copy_file(source: str, dest: str):
    path_name = shutil.copy(source, dest)
    print("Copy to " + path_name + " sucessfull")
    return path_name

SOURCE_DIR = "/usr/local/include/Pure2D/"
DEST_DIR = "usr/bin/include/Pure2D/"

HEADERS = [
    'Graphics/Buffers.h', 'Graphics/Camera.h', 'Graphics/Color.h',
    'Graphics/Font.h', 'Graphics/Mesh.h', 'Graphics/Quad.h', 'Graphics/Renderable.h',
    'Graphics/Renderer.h', 'Graphics/RenderTexture.h', 'Graphics/Shader.h', 'Graphics/SpriteBatch.h',
    'Graphics/Texture.h', 'Graphics/Transform.h', 'Graphics/Vertex.h',

    'Math/Constants.h', 'Math/Convert.h', 'Math/Manip.h', 'Math/Mat4.h',
    'Math/MatrixTransform.h', 'Math/Rect.h', 'Math/Vec2.h', 'Math/Vec3.h', 'Math/Vec4.h',

    'System/DataType.h', 'System/Defer.h', 'System/Logging.h', 'System/NonCopyable.h',
    'System/Time.h', 'System/Util.h',

    'Window/Keyboard.h', 'Window/Window.h', 'Window/WindowEvent.h',

    'Audio.h', 'Define.h', 'Graphics.h', 'Init.h', 'Pure2D.h', 'PureMath.h', 'System.h', 'Window.h'
]

for h in HEADERS:
    source = SOURCE_DIR + h
    dest = DEST_DIR + h
    split_path = h.split('/')
    sub_path = DEST_DIR + (split_path[0] if len(split_path) > 1 else '')
    if not os.path.exists(sub_path):
        os.makedirs(sub_path)

    copy_file(source, dest)

LIB_SOURCE = "/usr/local/lib/x86_64-linux-gnu/"
LIB_DEST = "/usr/bin/lib/x86_64-linux-gnu/"

if not os.path.exists(LIB_DEST):
    os.makedirs(LIB_DEST)

copy_file(LIB_SOURCE + 'libpure2d.so', LIB_DEST + 'libpure2d.so')


