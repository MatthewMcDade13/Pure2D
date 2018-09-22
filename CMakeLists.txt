cmake_minimum_required(VERSION 3.10)
project(Pure2D)

set(CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)

find_package(PkgConfig REQUIRED 3.3)

add_library(Pure2D SHARED
        Define.h
        Graphics.h Math.h System.h Window.h Init.h Init.cpp
        Window/Window.cpp Window/Window.h
        External/glad.c External/glad.h
        External/khrplatform.h
        External/stb_image.cpp External/stb_image.h
        Window/Keyboard.h Window/WindowEvent.h
        Math/Vec2.h Math/Vec3.h Math/Vec4.h
        Math/Mat4.cpp Math/Mat4.h
        Math/MatrixTransform.cpp Math/MatrixTransform.h
        Math/Rect.h
        Math/Manip.cpp Math/Manip.h
        Math/Convert.h
        Math/Constants.h
        Private/Convert.h
        Private/FileIO.cpp Private/FileIO.h
        Private/Shaders.h
        Private/Util.h
        Graphics/Buffers.cpp Graphics/Buffers.h
        Graphics/Camera.cpp Graphics/Camera.h
        Graphics/Shader.cpp Graphics/Shader.h
        Graphics/Sprite.cpp Graphics/Sprite.h
        Graphics/Texture.cpp Graphics/Texture.h
        Graphics/Vertex.h
        System/AssetManager.cpp System/AssetManager.h
        System/NonCopyable.h
        System/Time.cpp System/Time.h
        Graphics/Renderer.cpp Graphics/Renderer.h Pure2D.h System/Util.h Graphics/Transform.cpp Graphics/Transform.h)

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
        Graphics.h Math.h System.h Window.h Init.h Pure2D.h
        DESTINATION "include/${PROJECT_NAME}"
)

# WINDOW
install(
        FILES
        "Window/Window.h"
        "Window/WindowEvent.h"
        Window/Keyboard.h
        DESTINATION "include/${PROJECT_NAME}/Window"
)

# MATH
install(
        FILES
        "Math/Vec2.h" "Math/Vec3.h" "Math/Vec4.h"
        "Math/Mat4.h" "Math/MatrixTransform.h"
        "Math/Rect.h" "Math/Manip.h"
        "Math/Convert.h" "Math/Constants.h"
        DESTINATION "include/${PROJECT_NAME}/Math"
)

# GRAPHICS
install(
        FILES
        "Graphics/Buffers.h" "Graphics/Camera.h"
        "Graphics/Shader.h" "Graphics/Sprite.h"
        "Graphics/Texture.h" "Graphics/Vertex.h"
        "Graphics/Renderer.h"
        "Graphics/Transform.h"
        DESTINATION "include/${PROJECT_NAME}/Graphics"
)

# SYSTEM
install(
        FILES
        "System/AssetManager.h"
        "System/NonCopyable.h"
        "System/Time.h"
        "System/Util.h"
        DESTINATION "include/${PROJECT_NAME}/System"
)

install(FILES "${PROJECT_BINARY_DIR}/pure2d-config.cmake" DESTINATION lib/cmake/${PROJECT_NAME})