cmake_minimum_required(VERSION 3.14)
project(PlatformWindows VERSION 0.1.1)

add_library(
        PlatformWindows
        STATIC
        private/engine/platform/win32/win32_window.cpp
        private/engine/platform/win32/win32_platform.cpp
        private/engine/platform/win32/win32_gl_context.cpp
)

target_include_directories(
        PlatformWindows
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private"
)

target_compile_definitions(
        PlatformWindows
        PRIVATE PLATFORM_WIN32_BUILD_LIBRARY
)

target_link_libraries(PlatformWindows EngineCore opengl32)