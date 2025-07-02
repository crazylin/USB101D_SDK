#ifndef _USB101D_DEVICE_
#define _USB101D_DEVICE_

#include<windows.h>

//***********************************************************
// 用于AD采集的参数结构
typedef struct _USB101D_PARA_AD
{
	LONG Frequency;         // 采集频率,单位为Hz, [1, 1000000]
	LONG InputRange[4];		// 模拟量输入量程选择
	LONG Gains[4];			// 增益控制
	LONG M_Length;          // M段长度(字), 总的取值范围1-8M, 但是M加N长度不能大于8M
	LONG N_Length;          // N段长度(字), 总的取值范围1-8M, 但是M加N长度不能大于8M
	LONG TriggerMode;		// 触发模式选择
	LONG TriggerSource;		// 触发源选择
	LONG TriggerDir;		// 触发方向选择(正向/负向触发)
	LONG TrigLevelVolt;		// 触发电平(量程按模拟输入量程)
	LONG ClockSource;		// 时钟源选择(内/外时钟源)
	LONG bClockOutput;      // 允许时钟输出到CLKOUT,=TRUE:允许时钟输出, =FALSE:禁止时钟输出
	LONG SampleMode;		// 采样模式
} USB101D_PARA_AD, *PUSB101D_PARA_AD;

//***********************************************************
// AD参数USB101D_PARA_AD中的Gains[x]使用的硬件增益选项
const long USB101D_GAINS_1MULT			= 0x00; // 1倍增益
const long USB101D_GAINS_2MULT			= 0x01; // 2倍增益
const long USB101D_GAINS_4MULT			= 0x02; // 4倍增益
const long USB101D_GAINS_8MULT			= 0x03; // 8倍增益

//***********************************************************
// AD硬件参数USB101D_PARA_AD中的InputRange量程所使用的选项
const long USB101D_INPUT_N10000_P10000mV= 0x00; // ±10000mV

//***********************************************************
// AD硬件参数USB101D_PARA_AD中的TriggerMode成员变量所使用触发模式选项
const long USB101D_TRIGMODE_MIDL		= 0x00; // 中间触发		(有限采样有效)
const long USB101D_TRIGMODE_POST		= 0x01; // 后触发		(有限采样有效、连续触发采样)
const long USB101D_TRIGMODE_PRE			= 0x02; // 预触发		(有限采样有效)
const long USB101D_TRIGMODE_DELAY		= 0x03; // 硬件延时触发	(有限采样有效、连续触发采样)

//***********************************************************
// AD硬件参数USB101D_PARA_AD中的TriggerSource触发源信号所使用的选项
const long USB101D_TRIGMODE_SOFT		= 0x00; // 软件触发
const long USB101D_TRIGSRC_DTR			= 0x01; // 选择DTR作为触发源
const long USB101D_TRIGSRC_ATR			= 0x02; // 选择ATR作为触发源
const long USB101D_TRIGSRC_TRIGGER		= 0x03; // Trigger信号触发（用于多卡同步）

//***********************************************************
// AD硬件参数USB101D_PARA_AD中的TriggerDir触发方向所使用的选项
const long USB101D_TRIGDIR_NEGATIVE		= 0x00; // 负向触发(低电平/下降沿触发)
const long USB101D_TRIGDIR_POSITIVE		= 0x01; // 正向触发(高电平/上升沿触发)
const long USB101D_TRIGDIR_POSIT_NEGAT	= 0x02; // 正负向触发(高/低电平或上升/下降沿触发)

//***********************************************************
// AD硬件参数USB101D_PARA_AD中的ClockSource时钟源所使用的选项
const long USB101D_CLOCKSRC_IN			= 0x00; // 内部时钟定时触发
const long USB101D_CLOCKSRC_OUT			= 0x01; // 外部时钟定时触发(使用CN1上的CLKIN信号输入)

//***********************************************************
// AD硬件参数USB101D_PARA_AD中的SampleMode成员变量所使用采样模式选项
const long USB101D_SAMPMODE_CONTINUOUS	= 0x00; // 连续采样（触发模式无效）
const long USB101D_SAMPMODE_CONTS_TRIG	= 0x01; // 连续触发采样(中间触发 预触发无效)
const long USB101D_SAMPMODE_FINITE		= 0x02; // 有限采样

//*************************************************************************************
// 用于AD采样的实际硬件参数
typedef struct _USB101D_STATUS_AD     
{
	LONG bADEanble;	// AD是否已经使能，=TRUE:表示已使能，=FALSE:表示未使能
	LONG bTrigger;  // AD是否被触发，=TRUE:表示已被触发，=FALSE:表示未被触发
	LONG bComplete; // AD是否整个转换过程是否结束，=TRUE:表示已结束，=FALSE:表示未结束
	LONG bAheadTrig;// AD触发点是否提前，=TRUE:表示触发点提前，=FALSE:表示触发点未提前
	LONG lEndAddr;	// 数据完成的结束地址
} USB101D_STATUS_AD, *PUSB101D_STATUS_AD;


// #################### AI采样范围信息结构体(USB101D_AI_VOLT_RANGE_INFO) #######################
typedef struct _USB101D_VOLT_RANGE_INFO
{
	ULONG nSampleRange;		// 当前采样范围挡位号
	double fMaxVolt;			// 采样范围的最大电压值,单位:伏(V)
	double fMinVolt;			// 采样范围的最小电压值,单位:伏(V)
	double fCodeWidth;			// 编码宽度,单位:伏(V), 即每个单位码值所分配的电压值
	double fOffsetCode;		// 偏移码值,一般用于零偏校准,它代表的电压值等价于fOffsetVolt
	double fNeadCode;			// 电压换算所需码值，双极性为-2048,单级性为0
} USB101D_VOLT_RANGE_INFO, *PUSB101D_VOLT_RANGE_INFO;


//***********************************************************
// 驱动函数接口
#ifndef _USB101D_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 常规通用函数 #################################
	HANDLE DEVAPI FAR PASCAL USB101D_CreateDevice(int DeviceLgcID = 0); // 创建设备对象(该函数使用系统内逻辑设备ID）
	HANDLE DEVAPI FAR PASCAL USB101D_CreateDeviceEx(LONG DevicePhysID); // 创建设备对象(该函数使用物理ID最大255）
	BOOL DEVAPI FAR PASCAL USB101D_SetDevicePhysID(HANDLE hDevice, LONG DevicePhysID); // 设置当前设备的物理ID号
	BOOL DEVAPI FAR PASCAL USB101D_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID); // 取得当前设备的逻辑ID号和物理ID号
	BOOL DEVAPI FAR PASCAL USB101D_GetSerialNumber(HANDLE hDevice, PULONG SerialNumber);
	BOOL DEVAPI FAR PASCAL USB101D_ResetDevice(HANDLE hDevice);		 // 复位整个USB设备
    BOOL DEVAPI FAR PASCAL USB101D_ReleaseDevice(HANDLE hDevice);    // 设备句柄

	//####################### AD数据读取函数 #################################
	BOOL DEVAPI FAR PASCAL USB101D_ADCalibration(				// AD自动校准函数
									HANDLE hDevice);			// 设备对象句柄,它由CreateDevice函数创建
	
    BOOL DEVAPI FAR PASCAL USB101D_InitDeviceAD(				// 初始化设备,当返回TRUE后,设备即刻开始传输.
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建
									PUSB101D_PARA_AD pADPara);  // 硬件参数, 它仅在此函数中决定硬件状态	

	BOOL DEVAPI FAR PASCAL USB101D_StartDeviceAD(				// 在初始化之后，启动设备
									HANDLE hDevice);			// 设备对象句柄

	BOOL DEVAPI FAR PASCAL USB101D_SetDeviceTrigAD(				// 当设备使能允许后，产生软件触发事件（只有触发源为软件触发时有效）
									HANDLE hDevice);			// 设备对象句柄

	BOOL DEVAPI FAR PASCAL USB101D_GetDevStatusAD(				// 获取AD采集状态(有限采样有效)
									HANDLE hDevice,				// 设备句柄
									PUSB101D_STATUS_AD pADStatus);// AD状态

	ULONG DEVAPI FAR PASCAL USB101D_GetSDRAMSize(				// 返回板载DDR2大小，单位为Mb
									HANDLE hDevice);			// 设备句柄

	BOOL DEVAPI FAR PASCAL USB101D_StopDeviceAD(				// 在启动设备之后，暂停设备
									HANDLE hDevice);			// 设备对象句柄

    BOOL DEVAPI FAR PASCAL USB101D_ReadDeviceAD(				// 初始化设备后，即可用此函数读取设备上的AD数据
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建
									USHORT ADBuffer[],			// 将用于接受数据的用户缓冲区
									LONG nReadSizeWords,		// 读取AD数据的长度(字)  
									PLONG nRetSizeWords = NULL,	// 实际返回数据的长度(字)
									double fTimeout = 10.0);		// 超时时间,单位:秒(S)，非有限采样有效

    BOOL DEVAPI FAR PASCAL USB101D_ReleaseDeviceAD( HANDLE hDevice); // 停止AD采集，释放AD对象所占资源

	BOOL DEVAPI FAR PASCAL USB101D_GetVoltRangeInfo(			// 获得量程的各种信息
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									ULONG nChannel,			// AD物理通道号[0, 3]
									ULONG nSampleRange,		// 量程选择[0]
									PUSB101D_VOLT_RANGE_INFO pRangeInfo); // 量程信息

   	//################# AD的硬件参数操作函数 ########################	
	BOOL DEVAPI FAR PASCAL USB101D_SaveParaAD(HANDLE hDevice, PUSB101D_PARA_AD pADPara);  
    BOOL DEVAPI FAR PASCAL USB101D_LoadParaAD(HANDLE hDevice, PUSB101D_PARA_AD pADPara);
    BOOL DEVAPI FAR PASCAL USB101D_ResetParaAD(HANDLE hDevice, PUSB101D_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值

	//############################################################################
	BOOL DEVAPI FAR PASCAL USB101D_GetDevVersion(				// 获取设备固件及程序版本
									HANDLE hDevice,				// 设备对象句柄,它由CreateDevice函数创建
									PULONG pulFmwVersion,		// 固件版本
									PULONG pulDriverVersion);	// 驱动版本


	//############################ 线程操作函数 ################################
	HANDLE DEVAPI FAR PASCAL USB101D_CreateSystemEvent(void); 	// 创建内核系统事件对象
	BOOL DEVAPI FAR PASCAL USB101D_ReleaseSystemEvent(HANDLE hEvent); // 释放内核事件对象

#ifdef __cplusplus
}
#endif

// 自动包含驱动函数导入库
#ifndef _USB101D_DRIVER_
	#ifndef _WIN64
		#pragma comment(lib, "USB101D_32.lib")
		#pragma message("======== Welcome to use our art company's products!")
		#pragma message("======== Automatically linking with USB101D_32.dll...")
		#pragma message("======== Successfully linked with USB101D_32.dll")
	#else
		#pragma comment(lib, "USB101D_64.lib")
		#pragma message("======== Welcome to use our art company's products!")
		#pragma message("======== Automatically linking with USB101D_64.dll...")
		#pragma message("======== Successfully linked with USB101D_64.dll")
	#endif

#endif

#endif; // _USB101D_DEVICE_