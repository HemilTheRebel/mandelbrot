cmake_minimum_required(VERSION 3.17)
project(mandelbrot)

set(CMAKE_CXX_STANDARD 20)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(mandelbrot src/main.cpp src/core.cpp)
target_include_directories(mandelbrot PRIVATE include)
target_link_libraries(mandelbrot PRIVATE Threads::Threads)

enable_testing()
add_executable(my_test src/core.cpp test/test.cpp test/catch.cpp)
target_include_directories(my_test PRIVATE include)
target_link_libraries(my_test PRIVATE Threads::Threads)

add_test(NAME my_test COMMAND my_test)
