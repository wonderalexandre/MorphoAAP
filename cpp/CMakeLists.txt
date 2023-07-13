cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(aap VERSION 0.1.0)

# Include dir
include_directories(/usr/local/include)

# Src
AUX_SOURCE_DIRECTORY(src SRC_FILES)

# Headers
set(PROJECT_SOURCE_DIR "src")
set(PROJECT_INCLUDE_DIR "include/aap_bits")

# Source files
set(SOURCE_FILES
    ${PROJECT_INCLUDE_DIR}/NodeCT.hpp
    ${PROJECT_SOURCE_DIR}/NodeCT.cpp
    ${PROJECT_SOURCE_DIR}/AttributeComputedIncrementally.hpp
    ${PROJECT_SOURCE_DIR}/AttributeComputedIncrementally.cpp   
    ${PROJECT_INCLUDE_DIR}/ComponentTree.hpp
    ${PROJECT_SOURCE_DIR}/ComponentTree.cpp
    ${PROJECT_INCLUDE_DIR}/ComputerMSER.hpp
    ${PROJECT_SOURCE_DIR}/ComputerMSER.cpp
    ${PROJECT_INCLUDE_DIR}/AttributeProfile.hpp
    ${PROJECT_SOURCE_DIR}/AttributeProfile.cpp
    ${PROJECT_INCLUDE_DIR}/AttributeFilters.hpp
    ${PROJECT_SOURCE_DIR}/AttributeFilters.cpp
    
)

# Set up such that XCode organizes the files correctly
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

# Add library
add_library(aap SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(aap PRIVATE include/aap_bits)

# Install
install(TARGETS aap DESTINATION lib)

# Install the headers
install(FILES include/aap DESTINATION include)

# Create base directory
install(DIRECTORY include/aap_bits DESTINATION include)