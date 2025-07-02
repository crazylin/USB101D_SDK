#include "USB101DSDK.h"

// 开发便利宏
#ifndef SAFE_HANDLE
#define SAFE_HANDLE(h) ((h) != NULL && (h) != INVALID_HANDLE_VALUE)
#endif

extern "C" {

//———————————————— 基础设备管理 ————————————————
HANDLE USBSDK_Open(int deviceLgcID)
{
    return USB101D_CreateDevice(deviceLgcID);
}

HANDLE USBSDK_OpenPhys(int physId)
{
    return USB101D_CreateDeviceEx(physId);
}

BOOL USBSDK_Close(HANDLE hDevice)
{
    if (!SAFE_HANDLE(hDevice)) return FALSE;
    // 先保证停止并释放 AD
    USB101D_StopDeviceAD(hDevice);
    USB101D_ReleaseDeviceAD(hDevice);
    return USB101D_ReleaseDevice(hDevice);
}

//———————————————— AD 相关 ————————————————
BOOL USBSDK_ConfigAD(HANDLE hDevice, PUSB101D_PARA_AD pPara)
{
    if (!SAFE_HANDLE(hDevice)) return FALSE;

    USB101D_PARA_AD paraLocal = {0};
    if (pPara) {
        paraLocal = *pPara; // 用户自定义
    } else {
        // 构造一份默认参数
        paraLocal.Frequency   = 10000; // 10 kSPS
        for (int i = 0; i < 4; ++i) {
            paraLocal.InputRange[i] = USB101D_INPUT_N10000_P10000mV;
            paraLocal.Gains[i]      = USB101D_GAINS_1MULT;
        }
        paraLocal.M_Length     = 0;
        paraLocal.N_Length     = 1024;
        paraLocal.TriggerMode  = USB101D_TRIGMODE_POST;
        paraLocal.TriggerSource= USB101D_TRIGMODE_SOFT;
        paraLocal.TriggerDir   = USB101D_TRIGDIR_NEGATIVE;
        paraLocal.TrigLevelVolt= 0;
        paraLocal.ClockSource  = USB101D_CLOCKSRC_IN;
        paraLocal.bClockOutput = FALSE;
        paraLocal.SampleMode   = USB101D_SAMPMODE_CONTINUOUS;
    }

    return USB101D_InitDeviceAD(hDevice, &paraLocal);
}

BOOL USBSDK_StartAD(HANDLE hDevice)
{
    if (!SAFE_HANDLE(hDevice)) return FALSE;
    return USB101D_StartDeviceAD(hDevice);
}

BOOL USBSDK_TriggerAD(HANDLE hDevice)
{
    if (!SAFE_HANDLE(hDevice)) return FALSE;
    return USB101D_SetDeviceTrigAD(hDevice);
}

BOOL USBSDK_ReadAD(HANDLE hDevice,
                   USHORT *buffer,
                   LONG    wordCount,
                   PLONG   retWords,
                   double  timeoutSec)
{
    if (!SAFE_HANDLE(hDevice) || buffer == nullptr) return FALSE;
    return USB101D_ReadDeviceAD(hDevice, buffer, wordCount, retWords, timeoutSec);
}

BOOL USBSDK_StopAD(HANDLE hDevice)
{
    if (!SAFE_HANDLE(hDevice)) return FALSE;
    return USB101D_StopDeviceAD(hDevice);
}

//———————————————— 参数读取/保存 ————————————————
BOOL USBSDK_GetADPara(HANDLE hDevice, PUSB101D_PARA_AD pPara)
{
    if (!SAFE_HANDLE(hDevice) || pPara == nullptr) return FALSE;
    return USB101D_LoadParaAD(hDevice, pPara);
}

BOOL USBSDK_SaveADPara(HANDLE hDevice, PUSB101D_PARA_AD pPara)
{
    if (!SAFE_HANDLE(hDevice) || pPara == nullptr) return FALSE;
    return USB101D_SaveParaAD(hDevice, pPara);
}

//———————————————— 量程信息获取 ————————————————
BOOL USBSDK_GetVoltRangeInfo(HANDLE hDevice,
                             ULONG nChannel,
                             ULONG nSampleRange,
                             PUSB101D_VOLT_RANGE_INFO pInfo)
{
    if (!SAFE_HANDLE(hDevice) || pInfo == nullptr) return FALSE;
    return USB101D_GetVoltRangeInfo(hDevice, nChannel, nSampleRange, pInfo);
}

} // extern "C" 