
#=============================================================================
# Copyright 2001-2013 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# This module is shared by multiple languages; use include blocker.
if(__WINDOWS_CLANG)
  return()
endif()
set(__WINDOWS_CLANG 1)

#
# cmake" -G Ninja .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang
#
#if("x${CMAKE_C_SIMULATE_ID}" STREQUAL "xMSVC"
#    OR "x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC")
#  include(Platform/Windows-MSVC)
  include(Platform/Windows-GNU)
  macro(__windows_compiler_clang lang)
    __windows_compiler_gnu(${lang})
	
	set(CMAKE_AR "lib")
	
    # Create archiving rules to support large object file lists for static libraries.
    set(CMAKE_${lang}_ARCHIVE_CREATE "<CMAKE_AR> /OUT:<TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_${lang}_ARCHIVE_APPEND "<CMAKE_AR> q  <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_${lang}_ARCHIVE_FINISH "")

    set(CMAKE_GNULD_IMAGE_VERSION "")
    set(CMAKE_${lang}_CREATE_SHARED_LIBRARY
      "<CMAKE_${lang}_COMPILER> <CMAKE_SHARED_LIBRARY_${lang}_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS> -o <TARGET> -Wl,/implib:<TARGET_IMPLIB> ${CMAKE_GNULD_IMAGE_VERSION} <OBJECTS> <LINK_LIBRARIES>")
    set(CMAKE_${lang}_LINK_EXECUTABLE
      "<CMAKE_${lang}_COMPILER> <FLAGS> <CMAKE_${lang}_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> -Wl,/implib:<TARGET_IMPLIB> ${CMAKE_GNULD_IMAGE_VERSION} <LINK_LIBRARIES>")
	
  endmacro()
