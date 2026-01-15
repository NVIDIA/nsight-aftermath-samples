# ~~~
# Copyright (c) 2018 Valve Corporation
# Copyright (c) 2018 LunarG, Inc.
# Copyright (c) 2019-2025 NVIDIA Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ~~~

# CMake project initialization ---------------------------------------------------------------------------------------------------
# This section contains pre-project() initialization, and ends with the project() command.

cmake_minimum_required(VERSION 3.17)

project(VkHelloNsightAftermath)

# find_package(), include() and global project settings --------------------------------------------------------------------------


# User-interface declarations ----------------------------------------------------------------------------------------------------
# This section contains variables that affect development GUIs (e.g. CMake GUI and IDEs), such as option(), folders, and variables
# with the CACHE property.

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_VKHELLONSIGHTAFTERMATH "Build VkHelloNsightAftermath" ON)

if(APPLE)
    message(FATAL_ERROR "Apple platform is not supported by Nsight Aftermath")
endif()


# # Enable IDE GUI folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(USE_CCACHE "Use ccache" OFF)
if(USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    endif()
endif()

# ~~~
# Find Vulkan Headers and Loader
# Search order:
#  User-supplied CMAKE_PREFIX_PATH containing paths to the header and/or loader install dirs
#  CMake options VULKAN_HEADERS_INSTALL_DIR and/or VULKAN_LOADER_INSTALL_DIR
#  Env vars VULKAN_HEADERS_INSTALL_DIR and/or VULKAN_LOADER_INSTALL_DIR
#  If on MacOS
#   CMake option MOTLENVK_REPO_ROOT
#   Env vars MOLTENVK_REPO_ROOT
#  Fallback to FindVulkan operation using SDK install or system installed components.
# ~~~
set(VULKAN_HEADERS_INSTALL_DIR "HEADERS-NOTFOUND" CACHE PATH "Absolute path to a Vulkan-Headers install directory")
set(VULKAN_LOADER_INSTALL_DIR "LOADER-NOTFOUND" CACHE PATH "Absolute path to a Vulkan-Loader install directory")
if(WIN32 AND "${VULKAN_LOADER_INSTALL_DIR}" STREQUAL "LOADER-NOTFOUND")
    if(CMAKE_CL_64)
        set(VULKAN_LOADER_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/x64")
    else()
        set(VULKAN_LOADER_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/x86")
    endif()
endif()
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${VULKAN_HEADERS_INSTALL_DIR};${VULKAN_LOADER_INSTALL_DIR};
    $ENV{VULKAN_HEADERS_INSTALL_DIR};$ENV{VULKAN_LOADER_INSTALL_DIR})

message(STATUS "Using find_package to locate Vulkan")
find_package(Vulkan)
find_package(VulkanHeaders)
get_filename_component(Vulkan_LIBRARY_DIR ${Vulkan_LIBRARY} DIRECTORY)
message(STATUS "Vulkan FOUND = ${Vulkan_FOUND}")
message(STATUS "Vulkan Lib Dir = ${Vulkan_LIBRARY_DIR}")
message(STATUS "Vulkan Lib = ${Vulkan_LIBRARY}")
message(STATUS "Vulkan Headers Include = ${VulkanHeaders_INCLUDE_DIR}")
message(STATUS "Vulkan Headers Registry = ${VulkanRegistry_DIR}")

# ~~~
# Find Nsight Aftermath SDK Headers and Library
# ~~~
message(STATUS "Using find_package to locate Nsight Aftermath SDK")
find_package(NsightAftermath)
get_filename_component(NsightAftermath_LIBRARY_DIR ${NsightAftermath_LIBRARY} DIRECTORY)
message(STATUS "Nsight Aftermath FOUND = ${NsightAftermath_FOUND}")
message(STATUS "Nsight Aftermath Lib Dir = ${NsightAftermath_LIBRARY_DIR}")
message(STATUS "Nsight Aftermath Lib = ${NsightAftermath_LIBRARY}")
message(STATUS "Nsight Aftermath Headers Include = ${NsightAftermath_INCLUDE_DIR}")

include(GNUInstallDirs)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers")
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} -fno-strict-aliasing -fno-builtin-memcmp")

    # For GCC version 7.1 or greater, we need to disable the implicit fallthrough warning since there's no consistent way to satisfy
    # all compilers until they all accept the C++17 standard
    if(CMAKE_COMPILER_IS_GNUCC AND NOT (CMAKE_CXX_COMPILER_VERSION LESS 7.1))
        set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} -Wimplicit-fallthrough=0")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} -std=c++11 -fno-rtti")
    if(UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    endif()
endif()

set(VKHELLONSIGHTAFTERMATH_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/.. ${VulkanHeaders_INCLUDE_DIR})

# Locate shader tools (glslangValidator, spirv-remap/spirv-opt).
# Search order is hint paths first; if VULKAN_SDK is set, do not fall back to system PATH (/usr/bin, etc).
set(_VK_SHADER_TOOL_HINTS)
if(GLSLANG_INSTALL_DIR)
    list(APPEND _VK_SHADER_TOOL_HINTS "${GLSLANG_INSTALL_DIR}" "${GLSLANG_INSTALL_DIR}/bin")
endif()
if(NOT "$ENV{GLSLANG_INSTALL_DIR}" STREQUAL "")
    list(APPEND _VK_SHADER_TOOL_HINTS "$ENV{GLSLANG_INSTALL_DIR}" "$ENV{GLSLANG_INSTALL_DIR}/bin")
endif()
if(NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    list(APPEND _VK_SHADER_TOOL_HINTS "$ENV{VULKAN_SDK}/bin" "$ENV{VULKAN_SDK}/Bin" "$ENV{VULKAN_SDK}/Bin32")
endif()

if(NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    set(_VK_SHADER_TOOL_NO_DEFAULT_PATH NO_DEFAULT_PATH)
else()
    unset(_VK_SHADER_TOOL_NO_DEFAULT_PATH)
endif()

find_program(GLSLANG_VALIDATOR NAMES glslangValidator HINTS ${_VK_SHADER_TOOL_HINTS} ${_VK_SHADER_TOOL_NO_DEFAULT_PATH})
find_program(SPIRV_REMAP NAMES spirv-remap HINTS ${_VK_SHADER_TOOL_HINTS} ${_VK_SHADER_TOOL_NO_DEFAULT_PATH})
find_program(SPIRV_OPT NAMES spirv-opt HINTS ${_VK_SHADER_TOOL_HINTS} ${_VK_SHADER_TOOL_NO_DEFAULT_PATH})

if(GLSLANG_VALIDATOR)
    message(STATUS "Using glslangValidator: ${GLSLANG_VALIDATOR}")
else()
    message(FATAL_ERROR "glslangValidator not found. Please install Vulkan SDK tools or set VULKAN_SDK/GLSLANG_INSTALL_DIR.")
endif()

if(SPIRV_REMAP)
    message(STATUS "Using spirv-remap: ${SPIRV_REMAP}")
else()
    if(SPIRV_OPT)
        message(STATUS "spirv-remap not found; using spirv-opt to strip debug information as a replacement: ${SPIRV_OPT}")
    else()
        message(FATAL_ERROR "Neither spirv-remap nor spirv-opt found. Please install Vulkan SDK tools or set VULKAN_SDK/GLSLANG_INSTALL_DIR.")
    endif()
endif()
unset(_VK_SHADER_TOOL_HINTS)
unset(_VK_SHADER_TOOL_NO_DEFAULT_PATH)

if(UNIX) # i.e. Linux
    include(FindPkgConfig)
    option(BUILD_WSI_XCB_SUPPORT "Build XCB WSI support" ON)
    option(BUILD_WSI_XLIB_SUPPORT "Build Xlib WSI support" ON)
    option(BUILD_WSI_WAYLAND_SUPPORT "Build Wayland WSI support" ON)
    set(VKHELLONSIGHTAFTERMATH_WSI_SELECTION "XCB" CACHE STRING "Select WSI target for VkHelloNsightAftermath (XCB, XLIB, WAYLAND, DISPLAY)")

    if(BUILD_WSI_XCB_SUPPORT)
        find_package(XCB)
    endif()

    if(BUILD_WSI_XLIB_SUPPORT)
        find_package(X11)
    endif()

    if(BUILD_WSI_WAYLAND_SUPPORT)
        find_package(Wayland)
        if(Wayland_FOUND)
            include_directories(${WAYLAND_CLIENT_INCLUDE_DIR})
        endif()
    endif()
endif()

if(WIN32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DWIN32_LEAN_AND_MEAN -DNOMINMAX)
    if(NOT MSVC_VERSION LESS 1900)
        # Enable control flow guard
        message(STATUS "Building VkHelloNsightAftermath with control flow guard")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/guard:cf>")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /guard:cf")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /guard:cf")
    endif()
elseif(UNIX) # i.e. Linux
    if(NOT VKHELLONSIGHTAFTERMATH_WSI_SELECTION)
        set(VKHELLONSIGHTAFTERMATH_WSI_SELECTION "XCB")
    endif()

    if(VKHELLONSIGHTAFTERMATH_WSI_SELECTION STREQUAL "XCB")
        if(NOT BUILD_WSI_XCB_SUPPORT)
            message(FATAL_ERROR "Selected XCB for VkHelloNsightAftermath build but not building Xcb support")
        endif()
        if(NOT XCB_FOUND)
            message(FATAL_ERROR "Selected XCB for VkHelloNsightAftermath build but XCB package not found")
        endif()
        set(VKHELLONSIGHTAFTERMATH_INCLUDE_DIRS ${XCB_INCLUDE_DIRS} ${VKHELLONSIGHTAFTERMATH_INCLUDE_DIRS})
        link_libraries(${XCB_LIBRARIES})
        add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
    elseif(VKHELLONSIGHTAFTERMATH_WSI_SELECTION STREQUAL "XLIB")
        if(NOT BUILD_WSI_XLIB_SUPPORT)
            message(FATAL_ERROR "Selected XLIB for VkHelloNsightAftermath build but not building Xlib support")
        endif()
        if(NOT X11_FOUND)
            message(FATAL_ERROR "Selected XLIB for VkHelloNsightAftermath build but X11 package not found")
        endif()
        set(VKHELLONSIGHTAFTERMATH_INCLUDE_DIRS ${X11_INCLUDE_DIR} ${VKHELLONSIGHTAFTERMATH_INCLUDE_DIRS})
        link_libraries(${X11_LIBRARIES})
        add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
    elseif(VKHELLONSIGHTAFTERMATH_WSI_SELECTION STREQUAL "WAYLAND")
        if(NOT BUILD_WSI_WAYLAND_SUPPORT)
            message(FATAL_ERROR "Selected Wayland for VkHelloNsightAftermath build but not building Wayland support")
        endif()
        if(NOT Wayland_FOUND)
            message(FATAL_ERROR "Selected Wayland for VkHelloNsightAftermath build but Wayland package not found")
        endif()
        set(VKHELLONSIGHTAFTERMATH_INCLUDE_DIRS ${WAYLAND_CLIENT_INCLUDE_DIR} ${VKHELLONSIGHTAFTERMATH_INCLUDE_DIRS})
        link_libraries(${WAYLAND_CLIENT_LIBRARIES})
        add_definitions(-DVK_USE_PLATFORM_WAYLAND_KHR)
    elseif(VKHELLONSIGHTAFTERMATH_WSI_SELECTION STREQUAL "DISPLAY")
        add_definitions(-DVK_USE_PLATFORM_DISPLAY_KHR)
    else()
        message(FATAL_ERROR "Unrecognized value for VKHELLONSIGHTAFTERMATH_WSI_SELECTION: ${VKHELLONSIGHTAFTERMATH_WSI_SELECTION}")
    endif()

    link_libraries(${API_LOWERCASE} m)
else()
    message(FATAL_ERROR "Unsupported Platform!")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if(WIN32)
    # Use static MSVCRT libraries
    foreach(configuration
            in
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${configuration} MATCHES "/MD")
            string(REGEX
                   REPLACE "/MD"
                           "/MT"
                           ${configuration}
                           "${${configuration}}")
        endif()
    endforeach()

endif()

# Macro to compile shaders with SPIR-V optimization
macro(compile_shader SHADER_TYPE SPIRV_TOOL)
    add_custom_command(COMMENT "Compiling cube ${SHADER_TYPE} shader"
                       OUTPUT cube.${SHADER_TYPE}.spirv
                       COMMAND ${GLSLANG_VALIDATOR} -V -g -o $<TARGET_FILE_DIR:VkHelloNsightAftermath>/cube.${SHADER_TYPE}.full.spirv
                               ${PROJECT_SOURCE_DIR}/cube.${SHADER_TYPE}
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:VkHelloNsightAftermath>/cube.${SHADER_TYPE}.full.spirv ${CMAKE_BINARY_DIR}/cube.${SHADER_TYPE}.full.spirv
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:VkHelloNsightAftermath>/cube.${SHADER_TYPE}.full.spirv $<TARGET_FILE_DIR:VkHelloNsightAftermath>/cube.${SHADER_TYPE}.spirv
                       COMMAND ${CMAKE_COMMAND} -E chdir $<TARGET_FILE_DIR:VkHelloNsightAftermath> ${${SPIRV_TOOL}} ${ARGN}
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:VkHelloNsightAftermath>/cube.${SHADER_TYPE}.spirv ${CMAKE_BINARY_DIR}/cube.${SHADER_TYPE}.spirv
                       MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/cube.${SHADER_TYPE}
                       DEPENDS ${PROJECT_SOURCE_DIR}/cube.${SHADER_TYPE} ${GLSLANG_VALIDATOR} ${${SPIRV_TOOL}})
endmacro()

if(SPIRV_REMAP)
    # Strip debug information from SPIR-V
    compile_shader(vert SPIRV_REMAP --strip-all --input cube.vert.spirv --output ./)
    compile_shader(frag SPIRV_REMAP --strip-all --input cube.frag.spirv --output ./)
else()
    # Newer Vulkan SDKs removed spirv-remap. Use spirv-opt to strip debug information instead.
    compile_shader(vert SPIRV_OPT --strip-debug --strip-nonsemantic cube.vert.spirv -o cube.vert.spirv)
    compile_shader(frag SPIRV_OPT --strip-debug --strip-nonsemantic cube.frag.spirv -o cube.frag.spirv)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")
endif()

include_directories(${VKHELLONSIGHTAFTERMATH_INCLUDE_DIRS})

# ----------------------------------------------------------------------------
# VkHelloNsightAftermath

if(NOT WIN32)
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR})
        add_executable(VkHelloNsightAftermath
                       VkHelloNsightAftermath.cpp
                       NsightAftermathGpuCrashTracker.cpp
                       NsightAftermathShaderDatabase.cpp
                       cube.vert
                       cube.frag
                       cube.vert.spirv
                       cube.frag.spirv)
        target_link_libraries(VkHelloNsightAftermath PRIVATE Vulkan::Vulkan NsightAftermath::NsightAftermath dl)
        target_compile_options(VkHelloNsightAftermath PRIVATE -DVULKAN_HPP_DISABLE_IMPLICIT_RESULT_VALUE_CAST)

        # Add post-build step to copy Nsight Aftermath library to the installation directory
        add_custom_command(TARGET VkHelloNsightAftermath POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${NsightAftermath_LIBRARY}"
                    $<TARGET_FILE_DIR:VkHelloNsightAftermath>
            COMMENT "Copying Nsight Aftermath library to output directory")

        # Also install the library to the installation directory
        install(FILES "${NsightAftermath_LIBRARY}" DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
else()
    if(CMAKE_CL_64)
        set(LIB_DIR "Win64")
    else()
        set(LIB_DIR "Win32")
    endif()

    add_executable(VkHelloNsightAftermath
                   WIN32
                   VkHelloNsightAftermath.cpp
                   NsightAftermathGpuCrashTracker.cpp
                   NsightAftermathShaderDatabase.cpp
                   cube.vert
                   cube.frag
                   cube.vert.spirv
                   cube.frag.spirv)
    target_link_libraries(VkHelloNsightAftermath PRIVATE Vulkan::Vulkan NsightAftermath::NsightAftermath)
    target_compile_options(VkHelloNsightAftermath PRIVATE -DVULKAN_HPP_DISABLE_IMPLICIT_RESULT_VALUE_CAST)

    # Add post-build step to copy Nsight Aftermath DLL to the installation directory
    add_custom_command(TARGET VkHelloNsightAftermath POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${NsightAftermath_DLL}"
                $<TARGET_FILE_DIR:VkHelloNsightAftermath>
        COMMENT "Copying Nsight Aftermath DLL to output directory")

    # Also install the DLL to the installation directory
    install(FILES "${NsightAftermath_DLL}" DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

install(TARGETS VkHelloNsightAftermath RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
