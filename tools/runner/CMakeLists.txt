add_executable(c166-test-runner main.cpp)
target_compile_features(c166-test-runner PRIVATE cxx_std_17)
target_compile_options(c166-test-runner PRIVATE -Wall -Wextra -Wpedantic)
