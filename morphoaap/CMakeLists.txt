include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
   pybind11
    URL https://github.com/pybind/pybind11/archive/v2.3.0.tar.gz
    )

FetchContent_GetProperties(pybind11)

if(NOT pybind11_POPULATED)
    FetchContent_Populate(pybind11)
    add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
endif()


cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(morphoaap)

# Include dir
include_directories(/usr/local/include)

# Src
AUX_SOURCE_DIRECTORY(src SRC_FILES)

# Headers
set(PROJECT_SOURCE_DIR "src")
set(PROJECT_INCLUDE_DIR "include")


# Source files
set(SOURCE_FILES
    ${PROJECT_INCLUDE_DIR}/NodeCT.hpp
    ${PROJECT_SOURCE_DIR}/NodeCT.cpp
    ${PROJECT_INCLUDE_DIR}/AttributeComputedIncrementally.hpp
    ${PROJECT_SOURCE_DIR}/AttributeComputedIncrementally.cpp   
    ${PROJECT_INCLUDE_DIR}/ComponentTree.hpp
    ${PROJECT_SOURCE_DIR}/ComponentTree.cpp
    ${PROJECT_INCLUDE_DIR}/ComputerMSER.hpp
    ${PROJECT_SOURCE_DIR}/ComputerMSER.cpp
    ${PROJECT_INCLUDE_DIR}/AttributeProfile.hpp
    ${PROJECT_SOURCE_DIR}/AttributeProfile.cpp
    ${PROJECT_INCLUDE_DIR}/AttributeFilters.hpp
    ${PROJECT_SOURCE_DIR}/AttributeFilters.cpp
    ${PROJECT_INCLUDE_DIR}/AdjacencyRelation.hpp
    ${PROJECT_SOURCE_DIR}/AdjacencyRelation.cpp
)

# Set up such that XCode organizes the files correctly
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

# Add library
add_library(morphoaap SHARED ${SOURCE_FILES})


# Include directories
target_include_directories(morphoaap PRIVATE include)

# Install
install(TARGETS morphoaap DESTINATION lib)

# Install the headers
install(FILES include/morphoaap DESTINATION include)

# Create base directory
install(DIRECTORY include DESTINATION include)