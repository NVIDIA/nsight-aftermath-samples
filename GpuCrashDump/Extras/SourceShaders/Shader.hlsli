//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#ifndef SHADER_HLSLI
#define SHADER_HLSLI

cbuffer SceneConstantBuffer : register(b0)
{
    float4 offset;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

#endif // SHADER_HLSLI
