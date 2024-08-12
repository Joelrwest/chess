cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(GGP)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

set(BOOST_INCLUDE_LIBRARIES program_options)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.7z
  USES_TERMINAL_DOWNLOAD TRUE
  DOWNLOAD_NO_EXTRACT FALSE
)
FetchContent_MakeAvailable(Boost)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_library(engine src/move.cpp src/position.cpp src/fen_parser.cpp src/types.cpp)
target_compile_options(engine PUBLIC -Wall -Wextra -Wpedantic -Werror)

add_executable(debug src/debug.cpp)
target_link_libraries(debug PUBLIC engine)
target_link_libraries(debug PUBLIC ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_compile_options(debug PUBLIC -Wall -Wextra -Wpedantic -Werror)

enable_testing()

add_executable(
  perft
  test/perft.cpp
)

target_include_directories(perft PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

target_link_libraries(
  perft
  engine
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(perft)
