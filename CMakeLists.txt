project (SortBench)
cmake_minimum_required (VERSION 2.6)

if(MSVC)
  add_definitions( -D_CRT_SECURE_NO_WARNINGS )
  #add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

SET(sortbench_src sort.cc dualpivot_sort.cc sort.h drand48.h timsort.h 
				  quicksort.h dualpivotquick.h random.h slice.h)
ADD_EXECUTABLE(sortbench ${sortbench_src})
ADD_EXECUTABLE(swapbench swaptest.cc)
ADD_EXECUTABLE(count_op count_op.cc quicksort.h)
ADD_EXECUTABLE(radixsort radixsort.cc)

add_subdirectory(merge)

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