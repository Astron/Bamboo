cmake_minimum_required(VERSION 2.8)
project(Bamboo)
include_directories(src)

## Compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
   CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -Wall -Wextra")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -Wall")
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

## Handle Bison and Flex components
find_package(BISON)
if(BISON_FOUND)
  add_custom_command(
    SOURCE ${CMAKE_SOURCE_DIR}/src/dcfile/parser.ypp
    COMMAND ${BISON_EXECUTABLE}
    ARGS --defines=${CMAKE_SOURCE_DIR}/src/dcfile/parser-defs.h
         -o ${CMAKE_SOURCE_DIR}/src/dcfile/parser.cpp
         ${CMAKE_SOURCE_DIR}/src/dcfile/parser.ypp
    COMMENT "Generating dcfile/parser.cpp"
    OUTPUT ${CMAKE_SOURCE_DIR}/src/dcfile/parser.cpp
    MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/src/dcfile/parser.ypp)
endif(BISON_FOUND)

find_package(FLEX)
if(FLEX_FOUND)
  add_custom_command(
    SOURCE ${CMAKE_SOURCE_DIR}/src/dcfile/lexer.lpp
    COMMAND ${FLEX_EXECUTABLE}
    ARGS -o${CMAKE_SOURCE_DIR}/src/dcfile/lexer.cpp
         ${CMAKE_SOURCE_DIR}/src/dcfile/lexer.lpp
    COMMENT "Generating dcfile/lexer.cpp"
    OUTPUT ${CMAKE_SOURCE_DIR}/src/dcfile/lexer.cpp
    MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/src/dcfile/lexer.lpp)
endif(FLEX_FOUND)

set(MODULE_SOURCES
  src/module/DistributedType.cpp
  src/module/NumericType.cpp
  src/module/ArrayType.cpp
  src/module/Struct.cpp
  src/module/Class.cpp
  src/module/Method.cpp
  src/module/KeywordList.cpp
  src/module/Module.cpp
  src/module/Field.cpp
  src/module/MolecularField.cpp
  src/module/Parameter.cpp)

set(DCFILE_SOURCES
  src/dcfile/parser.cpp
  src/dcfile/lexer.cpp
  src/dcfile/write.cpp
  src/dcfile/format.cpp)

set(TRAITS_SOURCES
  src/traits/default.cpp
  src/traits/hashLegacy.cpp
  src/traits/HashGenerator.cpp
  src/traits/primes.cpp
  src/traits/traits.cpp)

set(WIRE_SOURCES
  src/wire/Datagram.cpp
  src/wire/DatagramIterator.cpp)

option(BUILD_SHARED_LIBS "Builds shared libraries instead of static libraries.")

set(BUILD_PACKAGE None
  CACHE STRING "Enables CPack to build packages.")
set_property(CACHE BUILD_PACKAGE PROPERTY STRINGS None Default)
if(NOT BUILD_PACKAGE MATCHES None)
  include(CPack)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Bamboo is an object-oriented generic message protocol.")
  set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/README.md)
endif()

option(BUILD_16BIT_SIZETAG "Compiles the bamboo library with 16-bit sizetags." true)
if(BUILD_16BIT_SIZETAG)
  add_library(bamboo
    ${MODULE_SOURCES}
    ${DCFILE_SOURCES}
    ${TRAITS_SOURCES}
    ${WIRE_SOURCES})
  SET_TARGET_PROPERTIES(bamboo PROPERTIES LINK_FLAGS -lfl)
endif()

option(BUILD_32BIT_SIZETAG "Compiles the bamboo library with 32-bit sizetags.")
if(BUILD_32BIT_SIZETAG)
  add_library(bamboo32
    ${MODULE_SOURCES}
    ${DCFILE_SOURCES}
    ${TRAITS_SOURCES}
    ${WIRE_SOURCES})
  target_link_libraries(bamboo32 -lfl)
  target_compile_definitions(bamboo32 PUBLIC BAMBOO_32BIT_SIZETAG)
endif()
