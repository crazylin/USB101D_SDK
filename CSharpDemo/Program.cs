using System;
using System.Runtime.InteropServices;
using System.Linq;

namespace USB101D_CSharpDemo
{
    internal static class NativeMethods
    {
        private const string DllName = "USB101DSDK.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr USBSDK_Open(int deviceLgcID);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_Close(IntPtr hDevice);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_ConfigAD(IntPtr hDevice, ref USB101D_PARA_AD pPara);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_StartAD(IntPtr hDevice);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_TriggerAD(IntPtr hDevice);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_ReadAD(IntPtr hDevice, ushort[] buffer, int wordCount, out int retWords, double timeoutSec);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_StopAD(IntPtr hDevice);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_GetADPara(IntPtr hDevice, ref USB101D_PARA_AD pPara);
        
        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_SaveADPara(IntPtr hDevice, ref USB101D_PARA_AD pPara);

        [DllImport(DllName, CallingConvention = CallingConvention.StdCall)]
        public static extern bool USBSDK_GetVoltRangeInfo(IntPtr hDevice,
                                                          uint nChannel,
                                                          uint nSampleRange,
                                                          out USB101D_VOLT_RANGE_INFO pInfo);
    }

    // 枚举定义
    internal enum Gain : int
    {
        X1 = 0, // 1倍增益
        X2 = 1, // 2倍增益
        X4 = 2, // 4倍增益
        X8 = 3  // 8倍增益
    }

    internal enum InputRange : int
    {
        N10000_P10000mV = 0
        // 如需更多量程，可在此扩展
    }

    internal enum TriggerMode : int { Mid = 0, Post = 1, Pre = 2, Delay = 3 }
    internal enum TriggerSource : int { Soft = 0, DTR = 1, ATR = 2, Trigger = 3 }
    internal enum TriggerDir : int { Negative = 0, Positive = 1, Both = 2 }
    internal enum ClockSource : int { Internal = 0, External = 1 }
    internal enum SampleMode : int { Continuous = 0, ConTrig = 1, Finite = 2 }

    // USB101D_PARA_AD 结构体（与 C 端保持一致）
    [StructLayout(LayoutKind.Sequential)]
    internal struct USB101D_PARA_AD
    {
        public int Frequency;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] public InputRange[] InputRange;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)] public Gain[] Gains;
        public int M_Length;
        public int N_Length;
        public TriggerMode   TriggerMode;
        public TriggerSource TriggerSource;
        public TriggerDir    TriggerDir;
        public int TrigLevelVolt;
        public ClockSource   ClockSource;
        public bool bClockOutput;
        public SampleMode    SampleMode;
    }

    // 采样范围信息结构体
    [StructLayout(LayoutKind.Sequential)]
    internal struct USB101D_VOLT_RANGE_INFO
    {
        public uint nSampleRange;
        public double fMaxVolt;
        public double fMinVolt;
        public double fCodeWidth;
        public double fOffsetCode;
        public double fNeadCode;
    }

    internal class Program
    {
        private const int ChannelCount = 4;       // 通道数量 1~4
        private const int BufferLen    = 4096;    // 读取缓冲长度（字）
        private const int FrequencyHz  = 1000;   // 采样率 (Hz)

        private static void Main()
        {
            Console.WriteLine($"通道数: {ChannelCount}, 缓冲长度: {BufferLen}, 采样率: {FrequencyHz}Hz");

            IntPtr dev = NativeMethods.USBSDK_Open(0);
            if (dev == IntPtr.Zero || dev == new IntPtr(-1))
            {
                Console.WriteLine("打开设备失败");
                return;
            }

            // ---------------- 先读取当前参数 ----------------
            var para = new USB101D_PARA_AD
            {
                InputRange = new InputRange[4],
                Gains = new Gain[4]
            };

            if (!NativeMethods.USBSDK_GetADPara(dev, ref para))
            {
                Console.WriteLine("读取现有参数失败");
                NativeMethods.USBSDK_Close(dev);
                return;
            }

            // 获取各通道电压换算参数
            var rangeInfo = new USB101D_VOLT_RANGE_INFO[ChannelCount];
            for (uint ch = 0; ch < ChannelCount; ch++)
            {
                if (!NativeMethods.USBSDK_GetVoltRangeInfo(dev, ch, (uint)para.InputRange[ch], out rangeInfo[ch]))
                {
                    Console.WriteLine($"获取通道 {ch} 量程信息失败");
                }
            }

            // 修改我们关注的参数
            para.Frequency = FrequencyHz;
            for (int i = 0; i < ChannelCount; i++)
            {
                para.InputRange[i] = InputRange.N10000_P10000mV;
                para.Gains[i] = Gain.X1; // 1 倍增益
            }

            para.M_Length = 0;
            para.N_Length = BufferLen;
            para.TriggerMode = TriggerMode.Post;
            para.TriggerSource = TriggerSource.Soft;
            para.TriggerDir = TriggerDir.Negative;
            para.TrigLevelVolt = 0;
            para.ClockSource = 0;
            para.bClockOutput = false;
            para.SampleMode = SampleMode.Continuous;

            // 应用新参数
            if (!NativeMethods.USBSDK_ConfigAD(dev, ref para))
            {
                Console.WriteLine("初始化设备失败");
                NativeMethods.USBSDK_Close(dev);
                return;
            }

            if (!NativeMethods.USBSDK_StartAD(dev) || !NativeMethods.USBSDK_TriggerAD(dev))
            {
                Console.WriteLine("启动/触发失败");
                NativeMethods.USBSDK_Close(dev);
                return;
            }

            ushort[] buffer = new ushort[BufferLen];
            int ret;

            Console.WriteLine("按任意键停止... (Esc 退出)");
            while (!Console.KeyAvailable)
            {
                if (!NativeMethods.USBSDK_ReadAD(dev, buffer, buffer.Length, out ret, 1.0))
                {
                    Console.WriteLine("读取失败");
                    break;
                }

                if (ret == 0) continue;

                int samplePerChannel = ret / ChannelCount;
                Console.WriteLine($"读取 {ret} 字 (约 {samplePerChannel} × {ChannelCount} 通道) ");

                // 仅演示前 64 组采样（256 字）
                int groupsToShow = Math.Min(samplePerChannel, 64);
                for (int iGrp = 0; iGrp < groupsToShow; iGrp++)
                {
                    for (int ch = 0; ch < ChannelCount; ch++)
                    {
                        int index = iGrp * ChannelCount + ch;
                        ushort raw = buffer[index];
                        double volt = (raw + rangeInfo[ch].fNeadCode - rangeInfo[ch].fOffsetCode) * rangeInfo[ch].fCodeWidth;
                        Console.Write($"CH{ch:D2}={volt:F6}V\t");
                    }
                    Console.WriteLine();
                }
                Console.WriteLine();
            }

            // 清理
            NativeMethods.USBSDK_StopAD(dev);
            NativeMethods.USBSDK_Close(dev);
        }
    }
}
