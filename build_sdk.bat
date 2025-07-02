@echo off
:: build_sdk.bat —— USB101DSDK 一键编译脚本 (x86 & x64, /MT)
:: 直接双击或在 cmd 中运行即可；需预装 VS 并保证下方 vcvarsall 路径正确。

rem === 1. VS 环境路径（如安装路径不同，请自行修改） ===
set VS_VCVARS="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"

if not exist %VS_VCVARS% (
    echo [ERROR] 未找到 vcvarsall.bat：%VS_VCVARS%
    pause
    exit /b 1
)

rem === 2. 定义公共变量 ===
set SRC_CPP=USB101D_SDK\USB101DSDK.cpp
set SRC_H=USB101D_SDK\USB101DSDK.h
set PUB_H=USB101D.h

rem 清理并创建输出目录
rd /s /q output 2>nul
md output\x86 2>nul
md output\x64 2>nul

rem =================  编译 32 位 =================
echo === 生成 x86 DLL ===
call %VS_VCVARS% x86 >nul
if errorlevel 1 goto :vcfail

cl /nologo /LD /MT /utf-8 /DUSB101D_SDK_EXPORTS /wd4819 /I. %SRC_CPP% USB101D_32.lib ^
    /link /OUT:output\x86\USB101DSDK.dll /IMPLIB:output\x86\USB101DSDK.lib
if errorlevel 1 goto :buildfail

copy /y %SRC_H%   output\x86\ >nul
copy /y %PUB_H%   output\x86\ >nul
copy /y USB101D_32.lib output\x86\ >nul

rem =================  编译 64 位 =================
echo === 生成 x64 DLL ===
call %VS_VCVARS% x64 >nul
if errorlevel 1 goto :vcfail

cl /nologo /LD /MT /utf-8 /DUSB101D_SDK_EXPORTS /wd4819 /I. %SRC_CPP% USB101D_64.lib ^
    /link /OUT:output\x64\USB101DSDK.dll /IMPLIB:output\x64\USB101DSDK.lib /MACHINE:X64
if errorlevel 1 goto :buildfail

copy /y %SRC_H%   output\x64\ >nul
copy /y %PUB_H%   output\x64\ >nul
copy /y USB101D_64.lib output\x64\ >nul

echo.
echo === 全部编译完成，文件位于 output\x86 与 output\x64 ===
pause
exit /b 0

:vcfail
echo [ERROR] vcvarsall 调用失败，未正确加载编译环境。
pause
exit /b 1

:buildfail
echo [ERROR] 编译失败，请检查上方 cl 输出。
pause
exit /b 1 