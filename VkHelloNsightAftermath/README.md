# Example for Nsight Aftermath GPU Crash Dump User Application Integration

This sample source code shows how to integrate Nsight Aftermath GPU crash dump
collection and decoding into a Vulkan application.

## Requirements

On Windows

* Windows 10 Version 1809 (October 2018 Update) or newer
* [CMake 3.13 or newer](https://cmake.org/download)
* [Vulkan-SDK (1.2.135 or newer)](https://vulkan.lunarg.com/sdk/home)
* [Nsight Aftermath SDK 2020.1 or newer](https://developer.nvidia.com/nsight-aftermath)
* [NVIDIA R445 display driver or newer](https://www.nvidia.com/Download/index.aspx)
* [Visual Studio 2017 or newer](https://www.visualstudio.com)

On Linux

* [CMake 3.5 or newer](https://cmake.org/download)
* [Vulkan-SDK (1.2.135 or newer)](https://vulkan.lunarg.com/sdk/home)
* [Nsight Aftermath SDK 2020.2 or newer](https://developer.nvidia.com/nsight-aftermath)
* [NVIDIA R455 display driver or newer](https://www.nvidia.com/Download/index.aspx)
* Officially tested on Ubuntu 16.04 and above

## Source Organization

* `VkHelloNsightAftermath.cpp`
  * Main implementation file (device creation, resource creation,
    rendering).
  * All additions for Nsight Aftermath code instrumentation are guarded with
    `#if defined(USE_NSIGHT_AFTERMATH)`.
* `NsightAftermathGpuCrashTracker.cpp`
  * Implements a simple GPU crash dump tracker showing the use of the Nsight
    Aftermath API for collecting and decoding GPU crash dumps.
* `NsightAftermathShaderDatabase.cpp`
  * Implements a very basic shader database as an example of how to provide
    shader binary data and shader debug data for shader instruction address
    mapping when decoding crash dumps.

NOTE: this sample code implements GPU crash dump collection and decoding in a
single application, but this is not the typical use case. Rather, users will
typically split this into a GPU crash dump collection phase, integrated into
the graphics application, and an offline decoding phase, implemented as an
offline GPU crash dump analysis tool.

## Building the Sample

On Windows

* Install the Vulkan SDK on your platform and make sure the environment
  variable `VULKAN_SDK` is set.
* Unpack the Nsight Aftermath SDK on your platform and make sure to set the
  `NSIGHT_AFTERMATH_SDK` environment variable to the directory containing the
  files.
* Run CMake to generate your build files (for example,
  `cmake -G "Visual Studio 15 2017 Win64" -S . -B .\Built`).
* Build the VkHelloNsightAftermath target from the generated
  Visual Studio solution (`Built\VkHelloNsightAftermath.sln`).

On Linux

* Install the Vulkan SDK on your platform and set up the `VULKAN_SDK`
  environment varible, e.g., run `source <Vulkan SDK dir>/setup-env.sh`.
* Unpack the Nsight Aftermath SDK on your platform and make sure to set the
  `NSIGHT_AFTERMATH_SDK` environment variable to the directory containing the
  files.
* Create a directory for the build artifacts under the VkHelloNsightAftermath
  directory, e.g. `mkdir built && cd built`.
* Run `cmake .. && make` to generate the Makefile and build the
  VkHelloNsightAftermath executable.

## Running the Sample

* On Windows, run `VkHelloNsightAftermath.exe`, if necessary, copy
  `GFSDK_Aftermath_Lib.x64.dll` to the working directory.
* On Linux, run `./VkHelloNsightAftermath`.
* The application will render a simple animated cube and will hang/TDR after
  a few seconds.
* A GPU crash dump file (`VkHelloNsightAftermath-<PID>-<COUNT>.nv-gpudmp`),
  a text file containing the GPU crash dump data decoded into JSON
  (`VkHelloNsightAftermath-<PID>-<COUNT>.nv-gpudmp.json`) and a shader
  debug information file (`shader-<IDENTIFIER>.nvdbg`) will be created in the
  application's working directory.
* Open the JSON file to inspect the data captured for the crash or use
  [Nsight Graphics](https://developer.nvidia.com/nsight-graphics) to open
  the GPU crash dump file.

## Copyright and Licenses

cf. LICENSE file.

This Code is based upon the `cube` demo from https://github.com/KhronosGroup/Vulkan-Tools/ (Copyright (c) 2015-2019 The Khronos Group Inc).

