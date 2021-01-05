cmake_minimum_required(VERSION 3.17)
project(RSProject)

set(CMAKE_CXX_STANDARD 17)

set( BOOST_VERSION 1.58 )

find_package( Boost ${BOOST_VERSION})

add_subdirectory(polyscope)
add_subdirectory(eigen)
add_subdirectory(geometry-central)

add_executable(RSProject tests/catch_amalgamated.hpp tests/catch_amalgamated.cpp tests/base_test.cpp utils.h utils.cpp DenseMatrix.h DenseMatrix.cpp SparseMatrix.h SparseMatrix.cpp Vertex.h Edge.h Vertex.cpp Edge.cpp Triangle.h Triangle.cpp Complex.h Complex.cpp utilTypes.h utilTypes.cpp)

target_link_libraries("${PROJECT_NAME}" PUBLIC polyscope)
target_link_libraries("${PROJECT_NAME}" PUBLIC eigen)
target_link_libraries("${PROJECT_NAME}" PUBLIC geometry-central)






