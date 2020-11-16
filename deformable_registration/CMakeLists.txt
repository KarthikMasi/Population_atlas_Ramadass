cmake_minimum_required(VERSION 3.7.1)

project (deformable_registration)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_libraries(stdc++fs)
find_package (ITK REQUIRED)
include (${ITK_USE_FILE})

add_executable (deformable_registration deformable_registration.cxx)

target_link_libraries (deformable_registration ${ITK_LIBRARIES})
