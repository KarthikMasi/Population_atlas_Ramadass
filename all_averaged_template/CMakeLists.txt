cmake_minimum_required(VERSION 3.7.1)

project (average_all_rough)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_libraries(stdc++fs)
find_package (ITK REQUIRED)
include (${ITK_USE_FILE})

add_executable (average_all_rough average_all_rough.cxx)

target_link_libraries (average_all_rough ${ITK_LIBRARIES})
