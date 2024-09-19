set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRCS 
    ${SRC_DIR}/logger.cpp
    ${SRC_DIR}/input_manager.cpp
    ${SRC_DIR}/mesh.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/texture.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/application.cpp
    ${SRC_DIR}/graphics.cpp
)

set(EXTERNAL_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/
)

set(EXTERNAL_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/stb_image.cpp
)

add_library(engine SHARED ${SRCS} ${EXTERNAL_SRCS})
target_compile_features(engine PRIVATE cxx_std_20)
target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
target_compile_definitions(engine PRIVATE
    $<$<CONFIG:Debug>:
        PERIA_PAINT_DEBUG=1>
)

if (WIN32)
    #todo
elseif (UNIX)
    find_package(SDL2 REQUIRED)
    target_include_directories(engine PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(engine PRIVATE ${SDL2_LIBRARIES})
endif()

target_include_directories(engine 
    PRIVATE 
    ${INCLUDE_DIR} 
    ${EXTERNAL_INCLUDES}
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/public 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm 
)
