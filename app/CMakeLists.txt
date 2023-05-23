set (SRCS
    main.cpp
)

add_executable(app ${SRCS})
target_compile_features(app PRIVATE cxx_std_17)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(app PRIVATE
    $<$<CONFIG:Debug>:
        PERIA_PAINT_DEBUG=1>
)

target_link_libraries(app PRIVATE engine)
