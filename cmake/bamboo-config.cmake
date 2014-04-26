# - Config file for the bamboo package
# It defines the following variables
#  BAMBOO_INCLUDE_DIRS - include directories for bamboo
#  BAMBOO_LIBRARY_DIRS - library directories for bamboo
#  BAMBOO_LIBRARIES    - libraries to link against

# Compute paths
get_filename_component(BAMBOO_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(BAMBOO_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include/bamboo")
set(BAMBOO_LIBRARY_DIRS ${BAMBOO_CMAKE_DIR})

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET bamboo AND NOT BAMBOO_BINARY_DIR)
  include("${BAMBOO_CMAKE_DIR}/bamboo-targets.cmake")
endif()

# These are IMPORTED targets created by BambooTargets.cmake
set(BAMBOO_LIBRARIES bamboo)
