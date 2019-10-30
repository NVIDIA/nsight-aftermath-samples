# Example for Nsight Aftermath GPU Crash Dump User Application Integration

This sample source code shows how to integrate Nsight Aftermath GPU crash dump
collection and decoding into a D3D12 application.

## Requirements
* Windows 10 Version 1809 (October 2018 Update)
* [Visual Studio 2017](https://www.visualstudio.com/)
* [Windows 10 October 2018 Update SDK (17763)](https://developer.microsoft.com/en-US/windows/downloads/windows-10-sdk)
* [Nsight Aftermath SDK](https://developer.nvidia.com/nsight-aftermath)
* [NVIDIA R440 driver (or newer)](https://www.nvidia.com/Download/index.aspx?lang=en-us)

## Source Organization
* `D3D12HelloNsightAftermath.cpp`
** Main implementation file (D3D12 device creation, resource creation,
   rendering).
** All additions for Nsight Aftermath code instrumentation are guarded with
   `#if defined(USE_NSIGHT_AFTERMATH)`.
* `NsightAftermathGpuCrashTracker.cpp`
** Implements a simple GPU crash dump tracker showing the use of the Nsight
   Aftermath API for collecting and decoding GPU crash dumps.
* `NsightAftermathShaderDatabase.cpp`
** Implements a very basic shader database as an example of how to provide
   shader binary data and shader debug data for shader instruction address
   mapping when decoding crash dumps.

NOTE: this sample code implements GPU crash dump collection and decoding in a
single application, but this is not the typical use case. Rather, users will
typically split this into a GPU crash dump collection phase, integrated into the
graphics application, and an offline decoding phase, implemented as an offline
GPU crash dump analysis tool.

## Building the Sample
* Open `NsightAftermath.props` in a text editor and update `NsightAftermathDir`
  to match the installation of your installed copy of the Nsight Aftermath SDK,
  if necessary.
* Open D3D12HelloNsightAftermath.vcxproj in Visual Studio.
* Build -> Build Solution.

## Running the Sample
* Run D3D12HelloNsightAftermath.exe, if necessary, copy `GFSDK_Aftermath_Lib.x64.dll`
  to the working directory.
* The application will render a simple animated triangle and will hang/TDR after
  a few seconds.
* A GPU crash dump file (D3D12HelloNsightAftermath-<PID>-<COUNT>.nv-gpudmp),
  a text file containing the GPU crash dump data decoded into JSON
  (`D3D12HelloNsightAftermath-<PID>-<COUNT>.nv-gpudmp.json`) and a shader
  debug information file (`shader-<IDENTIFIER>.nvdbg`) will be created in the
  application's working directory.
* Open the JSON file to inspect the data captured for the crash or use
  [Nsight Graphics](https://developer.nvidia.com/nsight-graphics) to open
  the GPU crash dump file.

## Copyright and Licenses
cf. LICENSE file.

This Code is based upon the HelloConstBuffers sample from https://github.com/microsoft/DirectX-Graphics-Samples (Copyright (c) 2015 Microsoft).

