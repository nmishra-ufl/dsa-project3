cmake_minimum_required(VERSION 3.27)
project(Project3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

add_executable(Project3 main.cpp)

#target_include_directories(Project3 PRIVATE "C:/SFML/include")
target_link_libraries(Project3 sfml-audio sfml-graphics sfml-window sfml-system)
