cmake_minimum_required(VERSION 3.17)
project(KMeans C)

set(CMAKE_C_STANDARD 99)

add_executable(KMeans kmeans.c)

target_link_libraries(KMeans PRIVATE m)