cmake_minimum_required(VERSION 3.14)
project(GenericCSVParser CXX)

option(ENABLE_TESTING "Enable Test Builds" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

include(cmake/Conan.cmake)

set(CONAN_BUILD_OPTION "never" CACHE STRING "Allow for building missing conan dependencies, default is: never")
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")

if (ENABLE_TESTING)
    set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES}
            gtest/1.13.0)
endif ()

run_conan()

find_package(Boost 1.80 COMPONENTS iostreams system REQUIRED)
find_package(fmt REQUIRED)

add_subdirectory(Source/GenericCSVParser)

if (${ENABLE_TESTING})
    find_package(GTest REQUIRED)
    message(STATUS "Enabling UnitTests")
    add_subdirectory(tests)
else ()
    message(STATUS "Disabling UnitTests")
endif ()

if (${BUILD_EXAMPLES})
    add_subdirectory(examples)
endif ()
