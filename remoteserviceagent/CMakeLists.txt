cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME RemoteServiceAgent)
set(CMAKE_CXX_STANDARD 11)

project(${PROJECT_NAME} LANGUAGES C CXX VERSION 1.0.1 DESCRIPTION "Remote Service Agent")

set(CMAKE_MACOSX_RPATH OFF)

# Write Version to Library
configure_file(${PROJECT_SOURCE_DIR}/include/version_config.h.in ${CMAKE_BINARY_DIR}/generated/version_config.h)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS thread chrono system REQUIRED)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Build Library
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

include(GenerateExportHeader)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CMAKE_BINARY_DIR}/generated/version_config.h)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories($<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/generated>)
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
generate_export_header(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/remoteserviceagent.h)

# Build Tests
set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)

if ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
        OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    message(STATUS "Building with llvm Code Coverage Tools")
    set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Building with lcov Code Coverage Tools")
    set(CMAKE_CXX_FLAGS "--coverage")
endif ()

file(GLOB TESTS ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(${TEST_PROJECT_NAME} ${TESTS})
message(STATUS "Linking against Boost Libraries: ${Boost_LIBRARIES}")
target_link_libraries(${TEST_PROJECT_NAME} PUBLIC ${PROJECT_NAME} gtest_main)

## Discover Tests
include(GoogleTest)
gtest_discover_tests(${TEST_PROJECT_NAME})

## Documentation Steps
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

## Install Steps
set(INCLUDE_INSTALL_DIR include/)
set(LIB_INSTALL_DIR lib/)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/${PROJECT_NAME}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION "${LIB_INSTALL_DIR}/${PROJECT_NAME}/cmake"
        PATH_VARS
        )

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${LIB_INSTALL_DIR}/cmake/${PROJECT_NAME}
        )

install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${LIB_INSTALL_DIR}/cmake/${PROJECT_NAME}
)
