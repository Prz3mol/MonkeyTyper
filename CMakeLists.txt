cmake_minimum_required(VERSION 3.29)
project(MTYPER)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)

FetchContent_MakeAvailable(SFML)

add_executable(MTYPER main.cpp)
target_link_libraries(MTYPER sfml-graphics)

