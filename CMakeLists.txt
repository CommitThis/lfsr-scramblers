cmake_minimum_required(VERSION 3.22)
project(ScramblingLFSRs)

# Disable the Google Benchmark requirement on Google Test
set(BENCHMARK_ENABLE_TESTING NO)

include(FetchContent)



FetchContent_Declare(
    googletest SYSTEM
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.13.x
    GIT_PROGRESS TRUE
    EXCLUDE_FROM_ALL
)

FetchContent_Declare(
    googlebenchmark SYSTEM
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG origin/main
    GIT_PROGRESS TRUE
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(googlebenchmark)



add_library(lfsr INTERFACE)
target_include_directories(lfsr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(lfsr INTERFACE cxx_std_20)


add_executable(test_lfsr test_lfsr.cpp)
target_link_libraries(test_lfsr 
    PRIVATE 
        gtest
        gtest_main
        lfsr)
target_compile_features(test_lfsr PRIVATE cxx_std_20)


add_executable(bench_lfsr bench_lfsr.cpp)
target_link_libraries(bench_lfsr 
    PRIVATE 
        benchmark::benchmark
        lfsr)
target_compile_features(test_lfsr PRIVATE cxx_std_20)


add_executable(tune_lfsr tune_lfsr.cpp)
target_link_libraries(tune_lfsr PRIVATE lfsr)
target_compile_features(tune_lfsr PRIVATE cxx_std_20)
