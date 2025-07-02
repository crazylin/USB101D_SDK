/*
 * USB101DSDK.h
 * 面向阿尔泰 USB101D 采集卡的 C 接口封装，可供 C/C++、C# 等多语言调用。
 * 仅依赖官方提供的 USB101D.h，与之保持二进制兼容。
 */

#ifndef USB101D_SDK_H
#define USB101D_SDK_H

#include <windows.h>
#include "USB101D.h"

#ifdef USB101D_SDK_EXPORTS
    #define USBSDK_API __declspec(dllexport)
#else
    #define USBSDK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ================= 基础设备管理 =================
// 打开设备（逻辑 ID）
USBSDK_API HANDLE USBSDK_Open(int deviceLgcID);
// 打开设备（物理 ID）。物理 ID 范围 0~255
USBSDK_API HANDLE USBSDK_OpenPhys(int physId);
// 关闭并释放设备
USBSDK_API BOOL   USBSDK_Close(HANDLE hDevice);

// ================= AD 采集控制 ===================
// 以默认参数初始化设备并获取当前参数到 pPara（可为 NULL）
USBSDK_API BOOL   USBSDK_ConfigAD(HANDLE hDevice, PUSB101D_PARA_AD pPara);
// 开始 AD 采集
USBSDK_API BOOL   USBSDK_StartAD(HANDLE hDevice);
// 软触发 AD（当触发源为软件触发时有效）
USBSDK_API BOOL   USBSDK_TriggerAD(HANDLE hDevice);
// 读取 AD 数据（wordCount 以 "字" 为单位，返回 retWords 实际读取数量）
USBSDK_API BOOL   USBSDK_ReadAD(HANDLE hDevice,
                                USHORT *buffer,
                                LONG    wordCount,
                                PLONG   retWords,
                                double  timeoutSec);
// 停止 AD
USBSDK_API BOOL   USBSDK_StopAD(HANDLE hDevice);

// ================= AD 参数持久化 ================
// 读取当前硬件中的 AD 参数到 pPara
USBSDK_API BOOL   USBSDK_GetADPara(HANDLE hDevice, PUSB101D_PARA_AD pPara);
// 保存 pPara 到硬件（掉电保存）
USBSDK_API BOOL   USBSDK_SaveADPara(HANDLE hDevice, PUSB101D_PARA_AD pPara);

// 获取指定通道及量程的范围信息
USBSDK_API BOOL   USBSDK_GetVoltRangeInfo(HANDLE hDevice,
                                          ULONG nChannel,
                                          ULONG nSampleRange,
                                          PUSB101D_VOLT_RANGE_INFO pInfo);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // USB101D_SDK_H 