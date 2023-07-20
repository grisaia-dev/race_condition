cmake_minimum_required(VERSION 3.22)
project(PBL LANGUAGES CXX)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(INCLUDES includes/PBL/PBL.hxx)
set(SOURCES sources/PBL/PBL.cxx)

add_library(${PROJECT_NAME} STATIC ${INCLUDES} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/includes)