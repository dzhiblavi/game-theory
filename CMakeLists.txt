cmake_minimum_required(VERSION 3.14)
project(game-theory)
set(CMAKE_CXX_STANDARD 17)

#add_compile_definitions(LOCAL)

add_executable(A A.cpp)
add_executable(C C.cpp)
add_executable(D D.cpp)
add_executable(H H.cpp)
add_executable(R R.cpp)

add_executable(B B.cpp)
add_executable(G G.cpp)
