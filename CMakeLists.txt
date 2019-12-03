cmake_minimum_required(VERSION 3.10)
project(Pure2D)

set(CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)

find_package(PkgConfig REQUIRED 3.3)

add_library(Pure2D SHARED

        Audio/Audio.h Audio/Audio.cpp
        Audio/AudioSource.h Audio/AudioSource.cpp
        Audio/Listener.h Audio/Listener.cpp
        Audio/Music.h Audio/Music.cpp
        Audio/SoundBuffer.h Audio/SoundBuffer.cpp

        Engine/Game.cpp Engine/Game.h

        External/glad.c External/glad.h
        External/khrplatform.h
        External/stb_image.cpp External/stb_image.h

        Graphics/Buffers.cpp Graphics/Buffers.h
        Graphics/Camera.cpp Graphics/Camera.h
        Graphics/Color.h
        Graphics/Font.h Graphics/Font.cpp
        Graphics/Mesh.h Graphics/Mesh.cpp
        Graphics/Quad.cpp Graphics/Quad.h
        Graphics/RenderTexture.h Graphics/RenderTexture.cpp
        Graphics/Renderable.h 
        Graphics/Renderer.cpp Graphics/Renderer.h 
        Graphics/Shader.cpp Graphics/Shader.h
        Graphics/SpriteBatch.cpp Graphics/SpriteBatch.h 
        Graphics/Texture.cpp Graphics/Texture.h
        Graphics/Transform.cpp Graphics/Transform.h
        Graphics/Vertex.h Graphics/Vertex.cpp

        Math/Constants.h
        Math/Convert.h
        Math/Manip.cpp Math/Manip.h
        Math/Mat4.cpp Math/Mat4.h
        Math/MatrixTransform.cpp Math/MatrixTransform.h
        Math/Rect.h
        Math/Vec2.h Math/Vec3.h Math/Vec4.h

        Private/Convert.h Private/Convert.cpp
        Private/Debug.h
        Private/ErrorHandling.h Private/ErrorHandling.cpp
        Private/FileIO.cpp Private/FileIO.h
        Private/GlContext.h Private/GlContext.cpp
        Private/Shaders.h
        Private/Util.h

        System/DataType.h
        System/Defer.h
        System/NonCopyable.h
        System/Time.cpp System/Time.h
        System/Util.h 

        Window/Keyboard.h Window/Keyboard.cpp
        Window/Window.h Window/Window.cpp
        Window/WindowEvent.h

        Define.h
        Graphics.h PureMath.h System.h Window.h Audio.h
        Pure2D.h 
        Init.h Init.cpp 
        )

pkg_search_module(GLFW REQUIRED glfw3)


target_include_directories(Pure2D PUBLIC Graphics Math System Window ${CMAKE_SOURCE_DIR})
target_include_directories(Pure2D PRIVATE Private External)
target_include_directories(Pure2D PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(Pure2D ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})


set(CMAKE_INSTALL_PREFIX /usr)
set(CONF_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})

configure_file(./pure2d-config.cmake.in "${PROJECT_BINARY_DIR}/pure2d-config.cmake")

message(STATUS "${PROJECT_BINARY_DIR}/pure2d-config.cmake")

install(TARGETS Pure2D DESTINATION ${CMAKE_INSTALL_LIBDIR})

# ROOT
install(
        FILES
        "Define.h"
        Graphics.h PureMath.h System.h Window.h Init.h Pure2D.h Audio.h
        DESTINATION "include/${PROJECT_NAME}"
)

# WINDOW
install(
        FILES
        Window/Keyboard.h
        Window/Window.h
        Window/WindowEvent.h

        DESTINATION "include/${PROJECT_NAME}/Window"
)

# MATH
install(
        FILES
        Math/Constants.h
        Math/Convert.h
        Math/Manip.h
        Math/Mat4.h
        Math/MatrixTransform.h
        Math/Rect.h
        Math/Vec2.h Math/Vec3.h Math/Vec4.h

        DESTINATION "include/${PROJECT_NAME}/Math"
)

# GRAPHICS
install(
        FILES
        Graphics/Buffers.h
        Graphics/Camera.h
        Graphics/Color.h
        Graphics/Font.h
        Graphics/Mesh.h
        Graphics/Quad.cpp Graphics/Quad.h
        Graphics/RenderTexture.h
        Graphics/Renderable.h 
        Graphics/Renderer.h 
        Graphics/Shader.h
        Graphics/SpriteBatch.h 
        Graphics/Texture.h
        Graphics/Transform.h
        Graphics/Vertex.h
        DESTINATION "include/${PROJECT_NAME}/Graphics"
)

# SYSTEM
install(
        FILES

        System/DataType.h
        System/Defer.h
        System/NonCopyable.h
        System/Time.h
        System/Util.h 

        DESTINATION "include/${PROJECT_NAME}/System"
)

# Engine
install(
	FILES
	"Engine/Game.h"
	DESTINATION "include/${PROJECT_NAME}/Engine"
)

# Audio
install(
        FILES

        Audio/Audio.h
        Audio/AudioSource.h
        Audio/Listener.h
        Audio/Music.h
        Audio/SoundBuffer.h
        DESTINATION "include/${PROJECT_NAME}/Audio"
)

install(FILES "${PROJECT_BINARY_DIR}/pure2d-config.cmake" DESTINATION lib/cmake/${PROJECT_NAME})