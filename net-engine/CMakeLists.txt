CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(test)

INCLUDE_DIRECTORIES(
                    ${PROJECT_SOURCE_DIR}/
                    ${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/util
                    ${PROJECT_SOURCE_DIR}/3rd/rapidjson/include
                    )

ADD_DEFINITIONS(-D__LINUX__)

FILE(GLOB_RECURSE SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cc
                           ${PROJECT_SOURCE_DIR}/util/*.cc)

ADD_EXECUTABLE(test ${SRC_LIST})
TARGET_LINK_LIBRARIES(test pthread)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -std=c++11")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

MESSAGE(STATUS "OUTPUT_PATH:" ${EXECUTABLE_OUTPUT_PATH})
