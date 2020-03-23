//*********************************************************
//
// Copyright (c) 2019-2020, NVIDIA CORPORATION. All rights reserved.
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

#pragma once

// Temporary definitions for the VK_NV_device_diagnostics_config extension
// Remove when the Vulkan SDK has picked up the latest Vulkan headers.

#define VK_NV_device_diagnostics_config 1
#define VK_NV_DEVICE_DIAGNOSTICS_CONFIG_SPEC_VERSION 1
#define VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME "VK_NV_device_diagnostics_config"
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV ((VkStructureType)1000300000)
#define VK_STRUCTURE_TYPE_DEVICE_DIAGNOSTICS_CONFIG_CREATE_INFO_NV ((VkStructureType)1000300001)

typedef enum VkDeviceDiagnosticsConfigFlagBitsNV {
    VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_SHADER_DEBUG_INFO_BIT_NV = 0x00000001,
    VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_RESOURCE_TRACKING_BIT_NV = 0x00000002,
    VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_AUTOMATIC_CHECKPOINTS_BIT_NV = 0x00000004,
    VK_DEVICE_DIAGNOSTICS_CONFIG_FLAG_BITS_MAX_ENUM_NV = 0x7FFFFFFF
} VkDeviceDiagnosticsConfigFlagBitsNV;

typedef VkFlags VkDeviceDiagnosticsConfigFlagsNV;

typedef struct VkPhysicalDeviceDiagnosticsConfigFeaturesNV {
    VkStructureType    sType;
    void *pNext;
    VkBool32           diagnosticsConfig;
} VkPhysicalDeviceDiagnosticsConfigFeaturesNV;

typedef struct VkDeviceDiagnosticsConfigCreateInfoNV {
    VkStructureType                     sType;
    const void *pNext;
    VkDeviceDiagnosticsConfigFlagsNV    flags;
} VkDeviceDiagnosticsConfigCreateInfoNV;

namespace vk
{
    namespace layout
    {
        struct DeviceDiagnosticsConfigCreateInfoNV
        {
        protected:
            VULKAN_HPP_CONSTEXPR DeviceDiagnosticsConfigCreateInfoNV(VkDeviceDiagnosticsConfigFlagsNV flags_ = VkDeviceDiagnosticsConfigFlagsNV()) VULKAN_HPP_NOEXCEPT
                : flags(flags_)
            {}

            DeviceDiagnosticsConfigCreateInfoNV(VkDeviceDiagnosticsConfigCreateInfoNV const & rhs) VULKAN_HPP_NOEXCEPT
            {
                *reinterpret_cast<VkDeviceDiagnosticsConfigCreateInfoNV*>(this) = rhs;
            }

            DeviceDiagnosticsConfigCreateInfoNV& operator=(VkDeviceDiagnosticsConfigCreateInfoNV const & rhs) VULKAN_HPP_NOEXCEPT
            {
                *reinterpret_cast<VkDeviceDiagnosticsConfigCreateInfoNV*>(this) = rhs;
                return *this;
            }

        public:
            vk::StructureType sType = vk::StructureType(VK_STRUCTURE_TYPE_DEVICE_DIAGNOSTICS_CONFIG_CREATE_INFO_NV);
            const void* pNext = nullptr;
            VkDeviceDiagnosticsConfigFlagsNV flags;
        };
        static_assert(sizeof(DeviceDiagnosticsConfigCreateInfoNV) == sizeof(VkDeviceDiagnosticsConfigCreateInfoNV), "layout struct and wrapper have different size!");
    }

    struct DeviceDiagnosticsConfigCreateInfoNV : public layout::DeviceDiagnosticsConfigCreateInfoNV
    {
        VULKAN_HPP_CONSTEXPR DeviceDiagnosticsConfigCreateInfoNV(VkDeviceDiagnosticsConfigFlagsNV flags = VkDeviceDiagnosticsConfigFlagsNV()) VULKAN_HPP_NOEXCEPT
            : layout::DeviceDiagnosticsConfigCreateInfoNV(flags)
        {}

        DeviceDiagnosticsConfigCreateInfoNV(VkDeviceDiagnosticsConfigCreateInfoNV const & rhs) VULKAN_HPP_NOEXCEPT
            : layout::DeviceDiagnosticsConfigCreateInfoNV(rhs)
        {}

        DeviceDiagnosticsConfigCreateInfoNV& operator=(VkDeviceDiagnosticsConfigCreateInfoNV const & rhs) VULKAN_HPP_NOEXCEPT
        {
            layout::DeviceDiagnosticsConfigCreateInfoNV::operator=(rhs);
            return *this;
        }

        DeviceDiagnosticsConfigCreateInfoNV & setPNext(const void* pNext_) VULKAN_HPP_NOEXCEPT
        {
            pNext = pNext_;
            return *this;
        }

        DeviceDiagnosticsConfigCreateInfoNV & setFlags(VkDeviceDiagnosticsConfigFlagsNV flags_) VULKAN_HPP_NOEXCEPT
        {
            flags = flags_;
            return *this;
        }

        operator VkDeviceDiagnosticsConfigCreateInfoNV const&() const VULKAN_HPP_NOEXCEPT
        {
            return *reinterpret_cast<const VkDeviceDiagnosticsConfigCreateInfoNV*>(this);
        }

        operator VkDeviceDiagnosticsConfigCreateInfoNV &() VULKAN_HPP_NOEXCEPT
        {
            return *reinterpret_cast<VkDeviceDiagnosticsConfigCreateInfoNV*>(this);
        }

        bool operator==(DeviceDiagnosticsConfigCreateInfoNV const& rhs) const VULKAN_HPP_NOEXCEPT
        {
            return (sType == rhs.sType)
                && (pNext == rhs.pNext)
                && (flags == rhs.flags);
        }

        bool operator!=(DeviceDiagnosticsConfigCreateInfoNV const& rhs) const VULKAN_HPP_NOEXCEPT
        {
            return !operator==(rhs);
        }

    private:
        using layout::DeviceDiagnosticsConfigCreateInfoNV::sType;
    };
    static_assert(sizeof(DeviceDiagnosticsConfigCreateInfoNV) == sizeof(VkDeviceDiagnosticsConfigCreateInfoNV), "struct and wrapper have different size!");
    static_assert(std::is_standard_layout<DeviceDiagnosticsConfigCreateInfoNV>::value, "struct wrapper is not a standard layout!");

    template <> struct isStructureChainValid<DeviceCreateInfo, DeviceDiagnosticsConfigCreateInfoNV> { enum { value = true }; };
}


