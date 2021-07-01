cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME cppdemo)

project(${PROJECT_NAME} LANGUAGES C CXX)

# the following line seems to allow local development
set(RemoteServiceAgent_SOURCE_DIR ${PROJECT_SOURCE_DIR}/remoteserviceagent)
set(RemoteServiceAgent_DIR ${RemoteServiceAgent_SOURCE_DIR}/cmake)

if ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
        OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    message(STATUS "Building with llvm Code Coverage Tools")
    set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Building with lcov Code Coverage Tools")
    set(CMAKE_CXX_FLAGS "--coverage")
endif ()

if(MSVC)
    foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
        if(${flag_var} MATCHES "/MDd")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MDd")
    endforeach(flag_var)
endif(MSVC)

add_subdirectory(remoteserviceagent)
add_subdirectory(instrument)
