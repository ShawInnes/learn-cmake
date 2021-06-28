cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME InstrumentDevice)

project(${PROJECT_NAME} LANGUAGES C CXX VERSION 1.0.0 DESCRIPTION "Instrument Device")

enable_testing()

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)

set(CMAKE_MACOSX_RPATH OFF)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package(RemoteServiceAgent REQUIRED)

include_directories(${COMMON_INCLUDES} ${RemoteServiceAgent_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC RemoteServiceAgent)
