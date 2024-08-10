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

add_library(engine src/bit_board.cpp src/position.cpp)
target_compile_options(engine PUBLIC -Wall -Wextra -Wpedantic -Werror)

add_executable(debug src/debug.cpp)
target_link_libraries(debug PUBLIC engine)
target_link_libraries(debug PUBLIC Boost::program_options)
target_compile_options(debug PUBLIC -Wall -Wextra -Wpedantic -Werror)
