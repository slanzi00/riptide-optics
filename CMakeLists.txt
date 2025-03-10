cmake_minimum_required(VERSION 2.31)
project(riptide-simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

string(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Werror=float-equal -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast")

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

add_executable(simulate main.cpp config.cpp construction.cpp physics.cpp generator.cpp action.cpp run.cpp detector.cpp)
target_link_libraries(simulate ${Geant4_LIBRARIES})
