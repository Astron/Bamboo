cmake_minimum_required(VERSION 2.8)
project(Bamboo)

## OS-specific flags
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

## Compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
   CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -Wall -Wextra")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -Wall -Wextra")
  # warnings are errors, release code should not have warnings
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -Werror")
  # debug flags, wall, wextra
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra")
endif()

if(CMAKE_GENERATOR STREQUAL Xcode)
  set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -std=c++0x -stdlib=libc++")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -std=c++0x -stdlib=libc++")
  set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -std=c++0x -stdlib=libc++")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_definitions(-std=c++11)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  message(STATUS "GCC Version: ${GCC_VERSION}")
  if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    add_definitions(-std=c++11) # Enable the new C++ standard
  else()
    add_definitions(-std=c++0x) # Enable the new C++ standard
  endif()
endif()

## Source files
include_directories(src)

# no dependencies
set(BITS_FILES
  src/bits/byteorder.h
  src/bits/sizetag.h
  src/bits/buffers.h
  src/bits/bits.cpp)
set(BITS_EXPORTED
  bits/byteorder.h
  bits/sizetag.h)
source_group("Bits" FILES ${BITS_FILES})

# depends: bits
set(MODULE_FILES
  src/module/Type.h
  src/module/Type.cpp
  src/module/TypeAlias.h
  src/module/Numeric.h
  src/module/Numeric.ipp
  src/module/Numeric.cpp
  src/module/Array.h
  src/module/Array.cpp
  src/module/Struct.h
  src/module/Struct.ipp
  src/module/Struct.cpp
  src/module/Class.h
  src/module/Class.cpp
  src/module/Method.h
  src/module/Method.cpp
  src/module/KeywordList.h
  src/module/KeywordList.cpp
  src/module/Module.h
  src/module/Module.ipp
  src/module/Module.cpp
  src/module/Field.h
  src/module/Field.cpp
  src/module/MolecularField.h
  src/module/MolecularField.cpp
  src/module/Numeric.h
  src/module/Numeric.ipp
  src/module/Numeric.cpp
  src/module/Parameter.h
  src/module/Parameter.cpp
  src/module/Value.h
  src/module/Value.cpp
  src/module/Value.ipp)
set(MODULE_EXPORTED
  module/module
  module/Type.h
  module/TypeAlias.h
  module/Numeric.h
  module/Numeric.ipp
  module/Array.h
  module/Struct.h
  module/Struct.ipp
  module/Class.h
  module/Method.h
  module/KeywordList.h
  module/Module.h
  module/Module.ipp
  module/Field.h
  module/MolecularField.h
  module/Numeric.h
  module/Numeric.ipp
  module/NumericRange.h
  module/Parameter.h
  module/Value.h
  module/Value.ipp)
source_group("Module" FILES ${MODULE_FILES})

# depends: module
set(TRAITS_FILES
  src/traits/hash_module.h
  src/traits/hash_legacy.cpp
  src/traits/HashGenerator.h
  src/traits/HashGenerator.cpp
  src/traits/primes.h
  src/traits/primes.cpp)
set(TRAITS_EXPORTED traits/hash_module.h)
source_group("Traits" FILES ${TRAITS_FILES})

# depends: bits, module
set(DCFILE_FILES
  src/dcfile/lexer.cpp
  src/dcfile/lexer.h
  src/dcfile/parser.cpp
  src/dcfile/parser.h)
set(DCFILE_EXPORTED
  dcfile/lexer.h
  dcfile/parser.h
  dcfile/token.h)
source_group("DCFile" FILES ${DCFILE_FILES})

# depends: bits, module
set(WIRE_FILES
  src/wire/Datagram.h
  src/wire/Datagram.cpp
  src/wire/DatagramIterator.h
  src/wire/DatagramIterator.cpp)
set(WIRE_EXPORTED
  wire/Datagram.h
  wire/DatagramIterator.h)
source_group("Wire" FILES ${WIRE_FILES})

set(BAMBOO_SOURCES
  ${BITS_FILES}
  ${MODULE_FILES}
  ${DCFILE_FILES}
  ${TRAITS_FILES}
  ${WIRE_FILES})
set(BAMBOO_HEADERS
  ${BITS_EXPORTED}
  ${MODULE_EXPORTED}
  ${DCFILE_EXPORTED}
  ${TRAITS_EXPORTED}
  ${WIRE_EXPORTED})

# Create include directory in build directory
add_custom_target(EXPORT_HEADERS ALL)
foreach(HEADER ${BAMBOO_HEADERS})
  add_custom_command(TARGET EXPORT_HEADERS PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/src/${HEADER}
           ${CMAKE_CURRENT_BINARY_DIR}/include/bamboo/${HEADER})
endforeach()

option(BUILD_SHARED_LIBS "Builds shared libraries instead of static libraries.")
option(BUILD_SHARED_AND_STATIC "Build both shared and static libraries.")
if(BUILD_SHARED_AND_STATIC)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL
    "Builds shared libraries instead of static libraries." FORCE)
  set(LIBRARY_TYPE SHARED)
endif()

set(BUILD_PACKAGE None
  CACHE STRING "Enables CPack to build packages.")
set_property(CACHE BUILD_PACKAGE PROPERTY STRINGS None Default)
if(NOT BUILD_PACKAGE MATCHES None)
  include(CPack)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Bamboo is a library for defining object-oriented message protocols.")
  set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
endif()

option(BUILD_16BIT_SIZETAG "Compiles the bamboo library with 16-bit sizetags." true)
if(BUILD_16BIT_SIZETAG)
  add_library(bamboo ${LIBRARY_TYPE} ${BAMBOO_SOURCES})
  install(TARGETS bamboo DESTINATION lib/bamboo EXPORT bamboo-targets)
  install(EXPORT bamboo-targets DESTINATION lib/bamboo)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/bamboo-config.cmake" DESTINATION lib/bamboo)
endif()

option(BUILD_32BIT_SIZETAG "Compiles the bamboo library with 32-bit sizetags.")
if(BUILD_32BIT_SIZETAG)
  add_library(bamboo32 ${LIBRARY_TYPE} ${BAMBOO_SOURCES})
  target_compile_definitions(bamboo32 PUBLIC BAMBOO_32BIT_SIZETAG)
  install(TARGETS bamboo32 DESTINATION lib/bamboo EXPORT bamboo32-targets)
  install(EXPORT bamboo32-targets DESTINATION lib/bamboo)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/bamboo32-config.cmake" DESTINATION lib/bamboo)
endif()

if(BUILD_SHARED_AND_STATIC)
  if(BUILD_16BIT_SIZETAG)
    add_library(bamboo-static STATIC ${BAMBOO_SOURCES})
    if(NOT WIN32)
      set_target_properties(bamboo-static PROPERTIES OUTPUT_NAME bamboo)
    endif()
    install(TARGETS bamboo-static DESTINATION lib/bamboo)
  endif()
  if(BUILD_32BIT_SIZETAG)
    add_library(bamboo32-static STATIC ${BAMBOO_SOURCES})

    target_compile_definitions(bamboo32-static PUBLIC BAMBOO_32BIT_SIZETAG)
    if(NOT WIN32)
      set_target_properties(bamboo32-static PROPERTIES OUTPUT_NAME bamboo32)
    endif()
    install(TARGETS bamboo32-static DESTINATION lib/bamboo)
  endif()
endif()

## Check for CxxTest to enable unittests
find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/include)
    enable_testing()
#    CXXTEST_ADD_TEST(dcfile_test dcfile_test.cpp
#                     ${CMAKE_CURRENT_SOURCE_DIR}/test/dcfile/ParseValueTests.h)
#    target_link_libraries(dcfile_test bamboo)
endif()

# Is Python installed, and should Python interfaces be generated?
find_package(PythonLibs)
find_package(PythonInterp)
if(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND AND (BUILD_SHARED_AND_STATIC OR BUILD_SHARED_LIBS))
  set(PYTHON_FOUND TRUE)
else()
  unset(PYTHON_FOUND)
endif()

option(BUILD_PYTHON "Compiles python bindings for " ${PYTHON_FOUND})
if(BUILD_PYTHON)
  if(NOT BUILD_SHARED_AND_STATIC AND NOT BUILD_SHARED_LIBS)
    message(FATAL_ERROR "Building python libraries requires shared libraries to be built.")
  endif()
  if(NOT PYTHONLIBS_FOUND)
    message(FATAL_ERROR "Building python bindings is enabled, but python library wasn't found.")
  endif()
  if(NOT PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Building python bindings is enabled, but python interpreter wasn't found.")
  endif()

  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/CheckPythonModule.cmake)
  check_python_module(pybindgen)
  if(PYTHON_PYBINDGEN_FOUND)
    if(BUILD_16BIT_SIZETAG)
      # Generate the 16-bit bindings
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pythonBindings.cpp
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/generate.py
                ${CMAKE_CURRENT_BINARY_DIR}/python/pythonBindings.cpp
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/generate.py
        IMPLICIT_DEPENDS Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/mappings.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python)
    endif()
    if(BUILD_32BIT_SIZETAG)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pythonBindings32.cpp
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/generate.py
                ${CMAKE_CURRENT_BINARY_DIR}/python/pythonBindings32.cpp
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/generate.py
        IMPLICIT_DEPENDS Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/mappings.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python)
    endif()

    if(BUILD_16BIT_SIZETAG AND BUILD_32BIT_SIZETAG)
      add_custom_target(
        update-python-prebuilt
        ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/python/pythonBindings.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/prebuilt/pythonBindings.cpp
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/python/pythonBindings32.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/prebuilt/pythonBindings32.cpp)
    endif()
  else()
    if(BUILD_16BIT_SIZETAG)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pythonBindings.cpp
        COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/prebuilt/pythonBindings.cpp
          ${CMAKE_CURRENT_BINARY_DIR}/python/pythonBindings.cpp
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/generate.py
        IMPLICIT_DEPENDS Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/mappings.py)
    endif()
    if(BUILD_32BIT_SIZETAG)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pythonBindings32.cpp
        COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/prebuilt/pythonBindings32.cpp
          ${CMAKE_CURRENT_BINARY_DIR}/python/pythonBindings32.cpp
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/generate.py
        IMPLICIT_DEPENDS Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/mappings.py)
    endif()
  endif()

  # Get the python install directory
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE}
      -c "from distutils.sysconfig import get_python_lib; print get_python_lib(True)"
      OUTPUT_VARIABLE PYTHON_ARCH_DIR)
  STRING(REGEX REPLACE "(\r?\n)+$" "" PYTHON_ARCH_DIR "${PYTHON_ARCH_DIR}")
  set(PYTHON_INSTALL_DIR "${PYTHON_ARCH_DIR}" CACHE STRING
    "Path to the Python architecture-dependent package directory.")

  # Create a working directory
  add_custom_target(python-directory ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/python)

  if(BUILD_16BIT_SIZETAG)
    # Build the 16-bit python module
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python/build/
      COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/setup.py build
      MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/pythonBindings.cpp
      IMPLICIT_DEPENDS
        Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/setup.py
        Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/generate.py
        Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/mappings.py
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python)
    add_custom_target(python-bindings ALL
      DEPENDS bamboo python-directory
        ${CMAKE_CURRENT_BINARY_DIR}/pythonBindings.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/python/build/)

    # Install the 16-bit python module
    install(CODE "
      message(STATUS \"Installing: ${PYTHON_ARCH_DIR}/bamboo.so\")
      execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/setup.py install --quiet
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python)")
  endif()
  if(BUILD_32BIT_SIZETAG)
    # Build the 32-bit python module
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python/build/
      COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/setup32.py build
      MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/pythonBindings32.cpp
      IMPLICIT_DEPENDS
        Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/setup32.py
        Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/generate.py
        Python ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/mappings.py
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python)
    add_custom_target(python-bindings32 ALL
      DEPENDS bamboo32 python-directory
        ${CMAKE_CURRENT_BINARY_DIR}/pythonBindings32.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/python/build/)

    # Install the 32-bit python module
    install(CODE "
      message(STATUS \"Installing: ${PYTHON_ARCH_DIR}/bamboo32.so\")
      execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/setup32.py install --quiet
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python)")
  endif()
endif()

# Install header files
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/bamboo
  DESTINATION include
  FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/bamboo
  DESTINATION include
  FILES_MATCHING PATTERN "*.ipp")

# Create symlinks on linux
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  function(generate_library_links file_ext)
    set(PREFIX "${CMAKE_INSTALL_PREFIX}/lib")
    if(BUILD_16BIT_SIZETAG)
      set(LINK16 "${PREFIX}/libbamboo${file_ext}")
      set(TARGET16 "${PREFIX}/bamboo/libbamboo${file_ext}")
      install(CODE "
        message(STATUS \"Generating: Symlink ${LINK16}\")
        execute_process(COMMAND ln -sf \"${TARGET16}\" \"${LINK16}\")")
    endif()
    if(BUILD_32BIT_SIZETAG)
      set(LINK32 "${PREFIX}/libbamboo32${file_ext}")
      set(TARGET32 "${PREFIX}/bamboo/libbamboo32${file_ext}")
      install(CODE "
        message(STATUS \"Generating: Symlink ${LINK32}\")
        execute_process(COMMAND ln -sf \"${TARGET32}\" \"${LINK32}\")")
    endif()
  endfunction()

  # Library links
  if(BUILD_SHARED_AND_STATIC)
    generate_library_links(".so")
    generate_library_links(".a")
  elseif(BUILD_SHARED_LIBS)
    generate_library_links(".so")
  else()
    generate_library_links(".a")
  endif()

  # Folder links
  if(BUILD_32BIT_SIZETAG)
    set(BAMBOO_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/bamboo")
    set(BAMBOO_LIB32_DIR "${CMAKE_INSTALL_PREFIX}/lib/bamboo32")
    install(CODE "
      message(STATUS \"Generating: Symlink ${BAMBOO_LIB32_DIR}\")
      execute_process(COMMAND ln -sf \"${BAMBOO_LIB_DIR}\" \"${BAMBOO_LIB32_DIR}\")")
  endif()
endif()
