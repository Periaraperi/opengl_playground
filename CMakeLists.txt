cmake_minimum_required(VERSION 3.15)

project(opengl_playground)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(EXTERNAL_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/
)

set(EXTERNAL_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/stb_image.cpp
)

set(SRCS_GENERAL
    ${SRC_DIR}/general/logger.cpp
    ${SRC_DIR}/general/input_manager.cpp
    ${SRC_DIR}/general/shader.cpp
    ${SRC_DIR}/general/texture.cpp
    ${SRC_DIR}/general/mesh.cpp
)

set(SRCS_APP
    ${SRC_DIR}/app/main.cpp
)

add_executable(app ${SRCS_APP} ${SRCS_GENERAL} ${EXTERNAL_SRCS})
target_compile_features(app PRIVATE cxx_std_17)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(app PRIVATE
    $<$<CONFIG:Debug>:
        PERIA_PAINT_DEBUG=1>
)

if (UNIX)
    find_package(SDL2 REQUIRED)
    target_include_directories(app PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(app PRIVATE ${SDL2_LIBRARIES})
endif ()

target_include_directories(app PRIVATE 
    ${INCLUDE_DIR}/general
    ${INCLUDE_DIR}/app
    ${EXTERNAL_INCLUDES}
)
