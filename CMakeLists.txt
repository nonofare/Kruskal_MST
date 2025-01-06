cmake_minimum_required(VERSION 3.30)
project(Kruskal)

set(CMAKE_CXX_STANDARD 20)

find_package(Python3 COMPONENTS Development REQUIRED)

add_executable(Kruskal main.cpp
        Point.h
        Graph.cpp
        Graph.h
        UnionFind.cpp
        UnionFind.h
        Drawer.cpp
)

target_include_directories(Kruskal PRIVATE ${Python3_INCLUDE_DIRS})

target_link_libraries(Kruskal PRIVATE ${Python3_LIBRARIES})
