# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindVulkan
# ----------
#
# Try to find Vulkan
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines :prop_tgt:`IMPORTED` target ``Vulkan::Vulkan``, if
# Vulkan has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   Vulkan_FOUND          - True if Vulkan was found
#   Vulkan_INCLUDE_DIRS   - include directories for Vulkan
#   Vulkan_LIBRARIES      - link against this library to use Vulkan
#
# The module will also define two cache variables::
#
#   Vulkan_INCLUDE_DIR    - the Vulkan include directory
#   Vulkan_LIBRARY        - the path to the Vulkan library
#

set(_VULKAN_SDK_NO_DEFAULT_PATH)
set(_VULKAN_SDK_INCLUDE_PATHS)
set(_VULKAN_SDK_LIBRARY_PATHS)

if(NOT "$ENV{VULKAN_SDK}" STREQUAL "")
  # If VULKAN_SDK is set, do not fall back to system include/library paths.
  set(_VULKAN_SDK_NO_DEFAULT_PATH NO_DEFAULT_PATH)
endif()

if(WIN32)
  if(NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    list(APPEND _VULKAN_SDK_INCLUDE_PATHS "$ENV{VULKAN_SDK}/Include")
  endif()

  find_path(Vulkan_INCLUDE_DIR
    NAMES vulkan/vulkan.h
    PATHS
      ${_VULKAN_SDK_INCLUDE_PATHS}
    ${_VULKAN_SDK_NO_DEFAULT_PATH}
    )

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    if(NOT "$ENV{VULKAN_SDK}" STREQUAL "")
      list(APPEND _VULKAN_SDK_LIBRARY_PATHS "$ENV{VULKAN_SDK}/Lib" "$ENV{VULKAN_SDK}/Bin")
    endif()
    find_library(Vulkan_LIBRARY
      NAMES vulkan-1
      PATHS
        ${_VULKAN_SDK_LIBRARY_PATHS}
      ${_VULKAN_SDK_NO_DEFAULT_PATH}
      )
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    if(NOT "$ENV{VULKAN_SDK}" STREQUAL "")
      list(APPEND _VULKAN_SDK_LIBRARY_PATHS "$ENV{VULKAN_SDK}/Lib32" "$ENV{VULKAN_SDK}/Bin32")
    endif()
    find_library(Vulkan_LIBRARY
      NAMES vulkan-1
      PATHS
        ${_VULKAN_SDK_LIBRARY_PATHS}
      ${_VULKAN_SDK_NO_DEFAULT_PATH}
      NO_SYSTEM_ENVIRONMENT_PATH
      )
  endif()
else()
  if(NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    list(APPEND _VULKAN_SDK_INCLUDE_PATHS "$ENV{VULKAN_SDK}/include")
    list(APPEND _VULKAN_SDK_LIBRARY_PATHS "$ENV{VULKAN_SDK}/lib")
  endif()

  find_path(Vulkan_INCLUDE_DIR
    NAMES vulkan/vulkan.h
    PATHS
      ${_VULKAN_SDK_INCLUDE_PATHS}
    ${_VULKAN_SDK_NO_DEFAULT_PATH}
    )
  find_library(Vulkan_LIBRARY
    NAMES vulkan
    PATHS
      ${_VULKAN_SDK_LIBRARY_PATHS}
    ${_VULKAN_SDK_NO_DEFAULT_PATH}
    )
endif()

unset(_VULKAN_SDK_NO_DEFAULT_PATH)
unset(_VULKAN_SDK_INCLUDE_PATHS)
unset(_VULKAN_SDK_LIBRARY_PATHS)

set(Vulkan_LIBRARIES ${Vulkan_LIBRARY})
set(Vulkan_INCLUDE_DIRS ${Vulkan_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vulkan
  DEFAULT_MSG
  Vulkan_LIBRARY Vulkan_INCLUDE_DIR)

mark_as_advanced(Vulkan_INCLUDE_DIR Vulkan_LIBRARY)

if(Vulkan_FOUND AND NOT TARGET Vulkan::Vulkan)
  add_library(Vulkan::Vulkan UNKNOWN IMPORTED)
  set_target_properties(Vulkan::Vulkan PROPERTIES
    IMPORTED_LOCATION "${Vulkan_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${Vulkan_INCLUDE_DIRS}")
endif()
