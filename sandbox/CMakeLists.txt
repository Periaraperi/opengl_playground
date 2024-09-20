set (SRCS
    sandbox.cpp
)

add_executable(sandbox ${SRCS})
target_compile_features(sandbox PRIVATE cxx_std_20)
target_compile_options(sandbox PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
target_compile_definitions(sandbox PRIVATE
    $<$<CONFIG:Debug>:
        PERIA_DEBUG=1>
)

target_link_libraries(sandbox PRIVATE engine)
