cmake_minimum_required(VERSION 2.6)

project(acl_lib)

file(GLOB HEADER_FILES "*.h" "*.hpp" "*.hxx")
set(HEADER_FILES ${HEADER_FILES})

include_directories(${inc_dir} ${Boost_INCLUDE_DIR})
#set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE CXX)
