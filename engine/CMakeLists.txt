set (ENGINE_IMPL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set (SRCS 
    ${ENGINE_IMPL_DIR}/application.cpp
    ${ENGINE_IMPL_DIR}/shader.cpp 
    ${ENGINE_IMPL_DIR}/vertex_array.cpp 
    ${ENGINE_IMPL_DIR}/graphics_impl.cpp)

set (EXTERNAL_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/stb_image.cpp
)

set (EXTERNAL_INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/
)

set (LIB_NAME "peria_engine")

add_library(${LIB_NAME} SHARED ${EXTERNAL_SRCS} ${SRCS})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)
target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
target_compile_definitions(${LIB_NAME} PRIVATE "$<$<CONFIG:Debug>:PERIA_DEBUG=1>")

if (WIN32)
    #todo
elseif (UNIX)
    find_package(SDL2 REQUIRED)
    target_include_directories(${LIB_NAME} 
        PRIVATE ${SDL2_INCLUDE_DIRS} ${ENGINE_IMPL_DIR} ${EXTERNAL_INCLUDES}
        PUBLIC ${PUBLIC_INCLUDE_DIR}
    )
    target_link_libraries(${LIB_NAME} PRIVATE ${SDL2_LIBRARIES})
endif()
