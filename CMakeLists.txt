cmake_minimum_required(VERSION 3.25)

project(opengl_playground)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (SRCS 
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vertex_array.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/texture.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sampler.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_manager.cpp 

    ${CMAKE_CURRENT_SOURCE_DIR}/src/demo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/model.cpp 
)

set (EXTERNAL_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/stb_image.cpp
    # IMGUI
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/src/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/src/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/src/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/src/imgui_impl_sdl2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/src/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/src/imgui_widgets.cpp
)

set (EXTERNAL_INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/include
)

add_executable(app ${EXTERNAL_SRCS} ${SRCS})
target_compile_features(app PRIVATE cxx_std_20)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
target_compile_definitions(app PRIVATE "$<$<CONFIG:Debug>:PERIA_DEBUG=1>")

if (WIN32)
    #todo
elseif (UNIX)
    find_package(SDL2 REQUIRED)
    target_include_directories(app 
        PRIVATE ${SDL2_INCLUDE_DIRS} ${EXTERNAL_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(app PRIVATE ${SDL2_LIBRARIES} /usr/lib64/libassimp.so)
endif()
