cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME cppdemo)

project(${PROJECT_NAME} LANGUAGES C CXX)

# Include Conan Package Management
include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# the following line seems to allow local development

set(RemoteServiceAgent_SOURCE_DIR ${PROJECT_SOURCE_DIR}/remoteserviceagent)
set(RemoteServiceAgent_DIR ${RemoteServiceAgent_SOURCE_DIR}/cmake)

add_subdirectory(remoteserviceagent)
add_subdirectory(instrument)
