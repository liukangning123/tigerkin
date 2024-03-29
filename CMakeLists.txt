cmake_minimum_required(VERSION 3.18)
project(tigerkin)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(/usr/local/include)
link_directories(/usr/local/lib64)

find_library(YAMLCPP yaml-cpp)
# message("***", ${YAMLCPP})  用于输出

set(LIB_SRC
	src/log.cpp
	src/util.cpp
	src/config.cpp
	)

add_library(tigerkin SHARED ${LIB_SRC})

add_executable(test_log tests/log_test.cpp)
add_dependencies(test_log tigerkin)
target_link_libraries(test_log tigerkin ${YAMLCPP})

add_executable(test_config tests/config_test.cpp)
add_dependencies(test_config tigerkin)
target_link_libraries(test_config tigerkin ${YAMLCPP})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)