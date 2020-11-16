cmake_minimum_required(VERSION 3.7.1)

project (affine_register)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_libraries(stdc++fs)
find_package (ITK REQUIRED)
include (${ITK_USE_FILE})

add_executable (affine_register affine_register.cxx)

target_link_libraries (affine_register ${ITK_LIBRARIES})
