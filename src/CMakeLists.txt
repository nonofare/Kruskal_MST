set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/kruskal_test.cpp
        ${PROJECT_SOURCE_DIR}/include/Graph.cpp
        ${PROJECT_SOURCE_DIR}/include/UnionFind.cpp
)

add_executable(run_test ${SOURCES})

target_include_directories(run_test PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(Python3 COMPONENTS Development REQUIRED)
target_include_directories(run_test PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(run_test PRIVATE ${Python3_LIBRARIES})