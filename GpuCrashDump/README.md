# Example Nsight Aftermath GPU Crash Dump

This directory contains an example GPU crash dump captured with the
D3D12HelloNsightAftermath example application. This GPU crash dump captures a
TDR timeout due to a very long running vertex shader.

The sample used for capturing the GPU crash dump is a very basic D3D12
application that renders only a single triangle. When inspecting the dump you
will notice only a single vertex shader warp in flight when the GPU hang
occured. This is because any fragment work from previous frames has already
retired from the shader units by the time the hang is detected and processed by
the CPU. In real applications, there will usually be many more active warps
listed, some executing warps without any problems or exceptions, and some
indicating the faulting warps.

## Files
* D3D12HelloNsightAftermath-12252-1.nv-gpudmp -- the GPU crash dump file.
* shader-59339c1ea8934740-00000210749de540.nvdbg -- the debug information file
  for the shader referenced in the GPU crash dump.
* Extras\D3D12HelloNsightAftermath.pdb -- the Program Data Base file necessary
  for resolving the call stacks captured in the GPU crash dump.
* Extras\Shaders -- shader object files required to resolve to the shader source line.
* Extras\SourceShaders -- shader source files required to show the shader source.

## Reading the GPU Crash Dump 
* Install [Nsight Graphics](https://developer.nvidia.com/nsight-graphics)
* Run Nsight Graphics from the start menu
* Open the Gpu Crash Dump Options dialog (`Tools->Options...->GPU Crash Dump`).
* In `Shader Debug Info paths` add the file path that contains the
  shader-59339c1ea8934740-00000210749de540.nvdbg file.
* In `Shader Source Paths` add the file path to Extras\SourceShaders.
* In `Shader Object Paths` add the file path to Extras\Shaders.
* In `PDB Search Paths` add the file path that contains
  Extras\D3D12HelloNsightAftermath.pdb.
* Close the dialog by clicking `OK`.
* Open the D3D12HelloNsightAftermath-12252-1.nv-gpudmp file
  (`File->Open Files...`).
* Detailed information on how to use Nsight Graphics to inspect Nsight
  Aftermath GPU crash dumps can be found in the
  [Nsight Graphics User Guide](https://docs.nvidia.com/nsight-graphics/UserGuide/index.html#gcd_inspector).

