# USB101D 采集卡轻量级 SDK

本 SDK 基于官方 `USB101D.h` 头文件和随卡提供的 `USB101D_32/64.dll`，
对常用 AD 采样流程进行了再次封装，提供了更加简洁、跨语言的调用接口。

## 目录结构

```
USB101D_SDK/           SDK 源码（C 语言接口）
    ├── USB101DSDK.h   头文件（需对外发布）
    └── USB101DSDK.cpp 实现（编译 DLL）
Demo/                  C++ 控制台示例
    └── ContinuousReadDemo.cpp
CSharpDemo/            C# 示例工程（.NET 6+ 可直接编译）
    └── Program.cs
```

## 编译 SDK

1. 使用 Visual Studio 2019/2022 新建 **Win32 动态链接库 (DLL)** 工程；
2. 将 `USB101DSDK.cpp`、`USB101DSDK.h` 与官方 `USB101D.h` 加入工程；
3. 在 *附加库目录* 中添加 `USB101D_32.lib`（或 64 位时 `USB101D_64.lib`）；
4. 定义宏 `USB101D_SDK_EXPORTS` 以导出函数；
5. 生成将得到 `USB101DSDK.dll`。

> ⚠️ SDK 本身并不包含设备驱动与固件，需确保官方驱动已正确安装。

## 运行 C++ Demo

```
cd Demo
cl ContinuousReadDemo.cpp /I.. /DWIN32 /link ..\USB101DSDK.lib
ContinuousReadDemo.exe
```

> 运行时需将 `USB101D_32.dll` 与 `USB101DSDK.dll` 置于同目录或系统 PATH 中。

## 运行 C# Demo

```
dotnet new console -o CSharpDemo -n USB101D_CSharpDemo
# 将 Program.cs 覆盖到生成工程内
cd CSharpDemo
# 引用不需要其他包，直接构建即可
 dotnet run
```

运行前同样需要保证两个 DLL 位于工作目录或 PATH。 