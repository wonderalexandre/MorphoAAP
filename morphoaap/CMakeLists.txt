include(FetchContent)

FetchContent_Declare(
  pybind11_sources
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.2
)

FetchContent_GetProperties(pybind11_sources)

if(NOT pybind11_sources_POPULATED)
  FetchContent_Populate(pybind11_sources)

  add_subdirectory(
    ${pybind11_sources_SOURCE_DIR}
    ${pybind11_sources_BINARY_DIR}
    )
endif()







cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(morphoaap VERSION 0.0.1)

# Include dir
#include_directories(/usr/local/include)

# Src
AUX_SOURCE_DIRECTORY(src SRC_FILES)

# Headers
set(PROJECT_SOURCE_DIR "src")
set(PROJECT_INCLUDE_DIR "include")

include_directories("${CMAKE_SOURCE_DIR}/morphoaap/include")
include_directories("${CMAKE_SOURCE_DIR}/morphoaap")

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
#add_library(morphoaap SHARED ${SOURCE_FILES})


# create python module
add_library(morphoaap
    SHARED ${SOURCE_FILES}
    MODULE
        aap.cpp
  )



  file (GLOB SOURCE_FILES "morphoaap/src/*.cpp")
  file (GLOB HEADER_FILES "morphoaap/include/*.hpp")
  file (GLOB PYTHON_FILES "morphoaap/*.cpp" "morphoaap/*.hpp")
  
  # Set up such that XCode organizes the files
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES} ${PYTHON_FILES} )
  
  include(pybind11.cmake)
  pybind11_add_module(morphoaap 
      ${SOURCE_FILES}
      ${HEADER_FILES}
      ${PYTHON_FILES}
  )


# workaround for problem with different names of some math
# functions on Windows MSYS
# this probably only concerns our CI setup on Appveyor
# and may not be needed generally
if(WIN32 AND (NOT MSVC))
  target_compile_definitions(morphoaap
    PUBLIC
      "_hypot=hypot"
    )
endif()

target_link_libraries(morphoaap
  PUBLIC
    pybind11::module
)

set_target_properties(aap-feature
  PROPERTIES
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
  )


install(TARGETS morphoaap
  LIBRARY DESTINATION morphoaap
)




# Include directories
target_include_directories(morphoaap PRIVATE include)

# Install
install(TARGETS morphoaap DESTINATION lib)

# Install the headers
install(FILES include/morphoaap DESTINATION include)

# Create base directory
install(DIRECTORY include DESTINATION include)