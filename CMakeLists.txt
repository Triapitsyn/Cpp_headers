cmake_minimum_required(VERSION 3.15)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(untitled main.cpp c.h solution.h matrix.h your_code.h profile.h header.h vector.h Complex.cpp Complex.h Rational.h Retry.h UniquePtr.h ContainerSerialization.h SharedPtr.h MathExpression.h Optional.h BiMap.h MyVector.h MySimpleIntList.h Heap.h BaseDijkstra.h BaseDSU.h "HashTable(Lists).h" "HashTable(Vector).h" "RedBlackTree(Insertions).h")