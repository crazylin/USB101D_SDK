# USB101D 采集卡轻量级 SDK

USB101D 是一款 **4 通道、最高 1 MS/s** 的 USB 数据采集卡。为方便二次开发，本仓库在官方驱动与库文件基础上，提供了一个 **轻量级 SDK** 及 **C# 示例工程**，统一了 32/64 位调用接口，并精简了使用流程。

> ⚠️ **使用前请确保已安装原厂驱动，并在 *设备管理器* 中能看到 USB101D 设备。**

---

## 目录结构

```text
USB101D_SDK/           # C 语言 SDK 源码（编译为 DLL）
│   ├─ USB101DSDK.h    # 对外发布的头文件
│   └─ USB101DSDK.cpp  # 实现文件
build_sdk.bat          # 一键编译脚本（x86 / x64, /MT）
output/                # 运行脚本后生成的文件
│   ├─ x86/            # 32 位 DLL、LIB、头文件
│   └─ x64/            # 64 位 DLL、LIB、头文件
CSharpDemo/            # .NET 8.0 示例工程
│   ├─ Program.cs
│   └─ *.csproj / bin / obj …
USB101D_32.lib         # 官方 32 位导入库
USB101D_64.lib         # 官方 64 位导入库
USB101D.h              # 官方头文件（未修改）
```

---

## 快速编译

仓库提供 **`build_sdk.bat`**，可一次生成 32/64 位 SDK：

```powershell
# 在 "VS 2022 开发者命令提示符" 或 PowerShell 中执行
cd USB101D_SDK          # 仓库根目录
.\build_sdk.bat         # 开始编译
```

脚本主要步骤：

1. 调用 `vcvarsall.bat` 配置 MSVC 环境；
2. 使用 `/MT` 选项编译 `USB101DSDK.cpp`，分别链接 `USB101D_32.lib` / `USB101D_64.lib`；
3. 将生成的 `USB101DSDK.dll`, `USB101DSDK.lib` 以及头文件复制到 `output/x86` 与 `output/x64`。

生成目录示例：

```text
output/x86/
    USB101DSDK.dll  USB101DSDK.lib  USB101DSDK.h  USB101D.h  USB101D_32.lib
output/x64/
    USB101DSDK.dll  USB101DSDK.lib  USB101DSDK.h  USB101D.h  USB101D_64.lib
```

若脚本提示找不到 `vcvarsall.bat`，请根据实际 VS 安装路径修改脚本顶部 `VS_VCVARS` 变量。

---

## C# 示例

```powershell
cd CSharpDemo
dotnet run --configuration Release
```

运行要求：

1. **`USB101DSDK.dll`** 及官方 **`USB101D_32/64.dll`** 能被找到（放在可执行文件旁或加入系统 `PATH`）。
2. 至少有一台 USB101D 设备已连接并处于空闲状态。

示例功能：

* 自动打开首台设备；
* 以 **1 kS/s** 连续采样 4 通道；
* 将原始码值转换为电压并实时打印。

---

## 常见问题 FAQ

| 问题 | 解决办法 |
| ---- | -------- |
| 程序提示 **"找不到 USB101D.dll"** | 检查是否已安装官方驱动，或将 DLL 复制到工作目录 / 系统 `PATH`。 |
| `USBSDK_Open` 返回 `NULL` | 设备可能被占用或驱动异常；尝试重新插拔设备或重启电脑。 |
| 链接错误 *LNK2019 无法解析外部符号* | 确认已链接 `USB101D_32.lib`（x86）或 `USB101D_64.lib`（x64）。 |

---

## 许可说明

本仓库仅对官方 SDK 进行**二次封装与示例演示**，版权归原厂商及作者所有。仅供学习、测试与二次开发参考，**禁止商业发布**。 