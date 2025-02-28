cmake_minimum_required(VERSION 3.29)
project(MonkeyTyper)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(Projekt_PJC main.cpp)

target_link_libraries(Projekt_PJC sfml-graphics sfml-window sfml-system)