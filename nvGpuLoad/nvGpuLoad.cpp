#include "stdafx.h"
#include "nvGpuLoad.h"
#include <nvapi.h>

extern "C"
{

    NvPhysicalGpuHandle             m_GpuHandle;

    __declspec(dllexport) void nvInit()
    {
        NvAPI_Initialize();

        NvPhysicalGpuHandle NvGpuHandles[NVAPI_MAX_PHYSICAL_GPUS] = { 0 };
        NvU32 NvGpuCount = 0;
        NvAPI_Status Status = NvAPI_EnumPhysicalGPUs(NvGpuHandles, &NvGpuCount);
        if (Status != NVAPI_OK || NvGpuCount == 0)
            return;
        m_GpuHandle = NvGpuHandles[0];
    }

	__declspec(dllexport) int getGpuLoad()
    {
        NV_GPU_DYNAMIC_PSTATES_INFO_EX  m_StateInfo = {};
        m_StateInfo.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
        NvAPI_GPU_GetDynamicPstatesInfoEx(m_GpuHandle, &m_StateInfo);
        return m_StateInfo.utilization[0].percentage;
	}

    __declspec(dllexport) int getGpuMemLoad(void)
    {
        NV_DISPLAY_DRIVER_MEMORY_INFO MemInfo = { 0 };
        MemInfo.version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER_3;
        NvAPI_Status Status = NvAPI_GPU_GetMemoryInfo(m_GpuHandle, &MemInfo);
        return int(100.0f * MemInfo.curAvailableDedicatedVideoMemory / MemInfo.availableDedicatedVideoMemory);
    }

}