set (SRCS
    batch.cpp
)

add_executable(playground ${SRCS})
target_compile_features(playground PRIVATE cxx_std_17)
target_compile_options(playground PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(playground PRIVATE
    $<$<CONFIG:Debug>:
        PERIA_PAINT_DEBUG=1>
)

target_link_libraries(playground PRIVATE engine)
