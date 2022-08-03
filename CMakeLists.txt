cmake_minimum_required(VERSION 3.23)
project(chanchan)

find_package(SQLite3 REQUIRED)

set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(example example/example.cc)
target_link_libraries(example SQLite::SQLite3)

find_package(benchmark REQUIRED)

set(BenchmarkSrc
        test/bench.cc
        )

foreach(File IN LISTS BenchmarkSrc)
    get_filename_component(ExeName ${File} NAME_WE)
    add_executable(${ExeName} ${File})
    target_link_libraries(${ExeName} benchmark::benchmark)
endforeach()
