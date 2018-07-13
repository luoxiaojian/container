project(container C CXX)
cmake_minimum_required(VERSION 2.8)

set(default_build_type "Release")

option(USE_NUMA "whether use NUMA." false)

if (USE_NUMA)
  message(STATUS "Use NUMA")
else()
  add_definitions(-DNO_NUMA)
  message(STATUS "Don't use NUMA")
endif()

include_directories(${CMAKE_SOURCE_DIR})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fprofile-arcs -ftest-coverage")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(TEST_FILES construct_perf dtor_perf push_back self_def vvector darray dvector resize string vvector_perf)

foreach (f ${TEST_FILES})
  message(STATUS "Found test - " ${f})
  add_executable(${f} test/${f}.cc)
endforeach(f ${TEST_FILES})
