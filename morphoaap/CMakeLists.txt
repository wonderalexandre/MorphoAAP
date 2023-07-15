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
    
)

# Set up such that XCode organizes the files correctly
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

# Add library
#add_library(morphoaap SHARED ${SOURCE_FILES})


# create python module
add_library(morphoaap
    SHARED ${SOURCE_FILES}
    MODULE
    morphoaap.cpp
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

set_target_properties(morphoaap
  PROPERTIES
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
  )



# Include directories
#target_include_directories(morphoaap PRIVATE include)

# Install
#install(TARGETS morphoaap DESTINATION lib)

# Install the headers
#install(FILES include/morphoaap DESTINATION include)

# Create base directory
#install(DIRECTORY include DESTINATION include)