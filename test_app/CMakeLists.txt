set (SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics.cpp
)

set (EXTERNAL_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/stb_image.cpp
)

set (EXTERNAL_INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/
)

add_executable(app ${EXTERNAL_SRCS} ${SRCS})
target_compile_features(app PRIVATE cxx_std_20)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Wshadow)

if (WIN32)
    #todo
elseif (UNIX)
    find_package(SDL2 REQUIRED)
    target_include_directories(app PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(app PRIVATE ${SDL2_LIBRARIES})
endif()

target_include_directories(app PRIVATE ${EXTERNAL_INCLUDES})
