cmake_minimum_required(VERSION 2.6)

set(project_name "access_list")
project(${project_name})

set(root_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -o0 -fno-elide-constructors -ggdb")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o0 -fno-elide-constructors -ggdb")
endif()

set(binary_name "${project_name}")
set(inc_dir ${root_dir}
	    ${CMAKE_CURRENT_SOURCE_DIR} 
	    ${Boost_INCLUDE_DIR}
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
include_directories(${inc_dir})
add_subdirectory(src)
add_subdirectory(tests)
