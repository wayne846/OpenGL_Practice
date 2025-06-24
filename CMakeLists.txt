
cmake_minimum_required(VERSION 3.10)
project(OpenGL_Practice)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include paths
include_directories(
    include
    include/GL
    include/GLFW
    include/glm
    include/imgui
    include/assimp
    src
)

# Source files
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h src/*.hpp)
file(GLOB_RECURSE INC_FILES include/*.cpp include/*.h include/*.hpp)
file(GLOB_RECURSE SHADER_FILES assets/shaders/*.*)

# Executable
add_executable(OpenGL_Practice ${SRC_FILES} ${INC_FILES} ${SHADER_FILES})

# Visual Studio source grouping
source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SRC_FILES})
source_group(TREE ${CMAKE_SOURCE_DIR}/include PREFIX "Include" FILES ${INC_FILES})
source_group(TREE ${CMAKE_SOURCE_DIR}/assets/shaders PREFIX "Shaders" FILES ${SHADER_FILES})

# Library paths
link_directories(${CMAKE_SOURCE_DIR}/lib/glew)
link_directories(${CMAKE_SOURCE_DIR}/lib/GLFW)
link_directories(${CMAKE_SOURCE_DIR}/lib/assimp)

# Link with libraries
target_link_libraries(OpenGL_Practice
    ${CMAKE_SOURCE_DIR}/lib/GLFW/glfw3.lib
    ${CMAKE_SOURCE_DIR}/lib/glew/glew32.lib
    ${CMAKE_SOURCE_DIR}/lib/assimp/assimp-vc143-mtd.lib
    opengl32
)

# 設定visual studio在編輯情況下的工作目錄與輸出目錄相同
set_target_properties(OpenGL_Practice PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)"
)

# 複製 DLL
file(GLOB DLL_SOURCE_PATHS "${CMAKE_SOURCE_DIR}/dll/*.dll")
foreach(DLL_PATH ${DLL_SOURCE_PATHS})
    add_custom_command(TARGET OpenGL_Practice POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL_PATH}
        ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)
    )
endforeach()

set(ASSETS_SOURCE_PATHS
    ${PROJECT_SOURCE_DIR}/assets
)
# 每次編譯完成後，複製資料夾到執行檔所在資料夾(debug/release)
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/assets
)

add_dependencies(OpenGL_Practice copy_assets)


# 設定起始專案
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OpenGL_Practice)
