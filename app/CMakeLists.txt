add_executable(app main.cpp)

target_compile_features(app PRIVATE cxx_std_20)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Wshadow)

target_link_libraries(app PRIVATE peria_engine)
