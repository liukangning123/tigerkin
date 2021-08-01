cmake_minimum_required(VERSION 3.18)
project(tigerkin)

set(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")
set(LIB_SRC
	src/log.cpp
	src/util.cpp
	)

add_library(tigerkin SHARED ${LIB_SRC})

add_executable(test tests/log_test.cpp)
add_dependencies(test tigerkin)
target_link_libraries(test tigerkin)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)