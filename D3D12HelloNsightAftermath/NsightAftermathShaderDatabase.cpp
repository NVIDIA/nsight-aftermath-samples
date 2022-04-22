//*********************************************************
//
// Copyright (c) 2019-2022, NVIDIA CORPORATION. All rights reserved.
// 
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// 
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//
//*********************************************************

#include "stdafx.h"

#include <fstream>
#include <iomanip>

#include <windows.h>

#include "NsightAftermathShaderDatabase.h"

//*********************************************************
// ShaderDatabase implementation
//*********************************************************

ShaderDatabase::ShaderDatabase()
    : m_shaderBinaries()
    , m_sourceShaderDebugData()
{
    // Add shader binaries to database
    const char* shaderBinaryDirs[] = { SHADERS_DIR, "Shaders" };
    for (size_t n = 0; n < sizeof(shaderBinaryDirs) / sizeof(const char*); ++n)
    {
        const std::string filePattern = std::string(shaderBinaryDirs[n]) + "/*.cso";

        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA(filePattern.c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                const std::string filePath = std::string(shaderBinaryDirs[n]) + "/" + findData.cFileName;
                AddShaderBinary(filePath.c_str());
            } while (FindNextFileA(hFind, &findData) != 0);
            FindClose(hFind);
        }
    }

    // Add shader debug data to database
    const char* shaderDebugDataDirs[] = { SHADERS_DIR, "Shaders" };
    const char* shaderDebugDataFileExts[] = { ".lld", ".pdb" };
    for (size_t n = 0; n < sizeof(shaderDebugDataDirs) / sizeof(const char*); ++n)
    {
        for (size_t m = 0; m < sizeof(shaderDebugDataFileExts) / sizeof(const char*); ++m)
        {
            const std::string filePattern = std::string(shaderDebugDataDirs[n]) + "/*" + shaderDebugDataFileExts[m];

            WIN32_FIND_DATAA findData;
            HANDLE hFind = FindFirstFileA(filePattern.c_str(), &findData);
            if (hFind != INVALID_HANDLE_VALUE)
            {
                do
                {
                    const std::string filePath = std::string(shaderDebugDataDirs[n]) + "/" + findData.cFileName;
                    AddSourceShaderDebugData(filePath.c_str(), findData.cFileName);
                } while (FindNextFileA(hFind, &findData) != 0);
                FindClose(hFind);
            }
        }
    }
}

ShaderDatabase::~ShaderDatabase()
{
}

bool ShaderDatabase::ReadFile(const char* filename, std::vector<uint8_t>& data)
{
    std::ifstream fs(filename, std::ios::in | std::ios::binary);
    if (!fs)
    {
        return false;
    }

    fs.seekg(0, std::ios::end);
    data.resize(fs.tellg());
    fs.seekg(0, std::ios::beg);
    fs.read(reinterpret_cast<char*>(data.data()), data.size());
    fs.close();

    return true;
}

void ShaderDatabase::AddShaderBinary(const char* filePath)
{
    // Read the shader bytecode from the file
    std::vector<uint8_t> data;
    if (!ReadFile(filePath, data))
    {
        return;
    }

   // Create shader hashes for the shader bytecode
    const D3D12_SHADER_BYTECODE shader{ data.data(), data.size() };
    GFSDK_Aftermath_ShaderBinaryHash shaderHash;
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetShaderHash(
        GFSDK_Aftermath_Version_API,
        &shader,
        &shaderHash));

    // Store the data for shader instruction address mapping when decoding GPU crash dumps.
    // cf. FindShaderBinary()
    m_shaderBinaries[shaderHash].swap(data);
}

void ShaderDatabase::AddSourceShaderDebugData(const char* filePath, const char* fileName)
{
    // Read the shader debug data from the file
    std::vector<uint8_t> data;
    if (!ReadFile(filePath, data))
    {
        return;
    }

    // Populate shader debug name.
    // The shaders used in this sample are compiled with compiler-generated debug data
    // file names. That means the debug data file's name matches the corresponding
    // shader's DebugName.
    // If shaders are built with user-defined debug data file names, the shader database
    // must maintain a mapping between the shader DebugName (queried from the shader
    // binary with GFSDK_Aftermath_GetShaderDebugName()) and the name of the file
    // containing the corresponding debug data.
    // Please see the documentation of GFSDK_Aftermath_GpuCrashDump_GenerateJSON() for
    // additional information.
    GFSDK_Aftermath_ShaderDebugName debugName;
    strncpy_s(debugName.name, fileName, sizeof(debugName.name) - 1);

    // Store the data for shader instruction address mapping when decoding GPU crash dumps.
    // cf. FindSourceShaderDebugData()
    m_sourceShaderDebugData[debugName].swap(data);
}

// Find a shader bytecode binary by shader hash.
bool ShaderDatabase::FindShaderBinary(const GFSDK_Aftermath_ShaderBinaryHash& shaderHash, std::vector<uint8_t>& shader) const
{
    // Find shader binary data for the shader hash
    auto i_shader = m_shaderBinaries.find(shaderHash);
    if (i_shader == m_shaderBinaries.end())
    {
        // Nothing found.
        return false;
    }

    shader = i_shader->second;
    return true;
}

// Find a source shader debug info by shader debug name generated by the DXC compiler.
bool ShaderDatabase::FindSourceShaderDebugData(const GFSDK_Aftermath_ShaderDebugName& shaderDebugName, std::vector<uint8_t>& debugData) const
{
    // Find shader debug data for the shader debug name.
    auto i_data = m_sourceShaderDebugData.find(shaderDebugName);
    if (i_data == m_sourceShaderDebugData.end())
    {
        // Nothing found.
        return false;
    }

    debugData = i_data->second;
    return true;
}
