set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRCS 
    ${SRC_DIR}/logger.cpp
    ${SRC_DIR}/input_manager.cpp
    ${SRC_DIR}/mesh.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/texture.cpp
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
target_compile_features(engine PRIVATE cxx_std_17)
target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(engine PRIVATE
    $<$<CONFIG:Debug>:
        PERIA_PAINT_DEBUG=1>
)

if (WIN32)
    #todo
elseif (UNIX)
    find_package(SDL2 REQUIRED)
    target_include_directories(engine PUBLIC ${SDL2_INCLUDE_DIRS}) # public droebit
    target_link_libraries(engine PUBLIC ${SDL2_LIBRARIES})
endif()

# will later change to PRIVATE
# need to set up good API
target_include_directories(engine PUBLIC ${INCLUDE_DIR} ${EXTERNAL_INCLUDES})
