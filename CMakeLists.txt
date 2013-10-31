project (SortBench)
cmake_minimum_required (VERSION 2.6)

SET(sortbench_src sort.cc dualpivot_sort.cc sort.h drand48.h timsort.h 
				  quicksort.h dualpivotquick.h)
ADD_EXECUTABLE(sortbench ${sortbench_src})
ADD_EXECUTABLE(swapbench swaptest.cc)


MESSAGE(STATUS "")
MESSAGE(STATUS "")
MESSAGE(STATUS "configuration summary:")
MESSAGE(STATUS "")
MESSAGE(STATUS "  C compiler ...................... : ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "  C++ compiler .................... : ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "  C compiler flags ................ : ${CMAKE_C_FLAGS}${cfsp}${CMAKE_C_FLAGS_${buildtype}}")
MESSAGE(STATUS "  C++ compiler flags .............. : ${CMAKE_CXX_FLAGS}${cxxfsp}${CMAKE_CXX_FLAGS_${buildtype}}")
MESSAGE(STATUS "")
MESSAGE(STATUS "  BINARY dir ...................... : ${SortBench_BINARY_DIR}")
MESSAGE(STATUS "  SOURCE dir ...................... : ${SortBench_SOURCE_DIR}")
# Print out CMake info
MESSAGE(STATUS "  CMAKE_GENERATOR ................. : ${CMAKE_GENERATOR}")
MESSAGE(STATUS "  CMAKE_SOURCE_DIR ................ : ${CMAKE_SOURCE_DIR}")

# Print out system information
MESSAGE(STATUS "  CMAKE_SYSTEM .................... : ${CMAKE_SYSTEM}")
MESSAGE(STATUS "  CMAKE_SYSTEM_PROCESSOR .......... : ${CMAKE_SYSTEM_PROCESSOR}")
MESSAGE(STATUS "  BUILD ............................: ${CMAKE_BUILD_TYPE}")