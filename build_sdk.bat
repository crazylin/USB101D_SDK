@echo off
:: build_sdk.bat ���� USB101DSDK һ������ű� (x86 & x64, /MT)
:: ֱ��˫������ cmd �����м��ɣ���Ԥװ VS ����֤�·� vcvarsall ·����ȷ��

rem === 1. VS ����·�����簲װ·����ͬ���������޸ģ� ===
set VS_VCVARS="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"

if not exist %VS_VCVARS% (
    echo [ERROR] δ�ҵ� vcvarsall.bat��%VS_VCVARS%
    pause
    exit /b 1
)

rem === 2. ���幫������ ===
set SRC_CPP=USB101D_SDK\USB101DSDK.cpp
set SRC_H=USB101D_SDK\USB101DSDK.h
set PUB_H=USB101D.h

rem �����������Ŀ¼
rd /s /q output 2>nul
md output\x86 2>nul
md output\x64 2>nul

rem =================  ���� 32 λ =================
echo === ���� x86 DLL ===
call %VS_VCVARS% x86 >nul
if errorlevel 1 goto :vcfail

cl /nologo /LD /MT /utf-8 /DUSB101D_SDK_EXPORTS /wd4819 /I. %SRC_CPP% USB101D_32.lib ^
    /link /OUT:output\x86\USB101DSDK.dll /IMPLIB:output\x86\USB101DSDK.lib
if errorlevel 1 goto :buildfail

copy /y %SRC_H%   output\x86\ >nul
copy /y %PUB_H%   output\x86\ >nul
copy /y USB101D_32.lib output\x86\ >nul

rem =================  ���� 64 λ =================
echo === ���� x64 DLL ===
call %VS_VCVARS% x64 >nul
if errorlevel 1 goto :vcfail

cl /nologo /LD /MT /utf-8 /DUSB101D_SDK_EXPORTS /wd4819 /I. %SRC_CPP% USB101D_64.lib ^
    /link /OUT:output\x64\USB101DSDK.dll /IMPLIB:output\x64\USB101DSDK.lib /MACHINE:X64
if errorlevel 1 goto :buildfail

copy /y %SRC_H%   output\x64\ >nul
copy /y %PUB_H%   output\x64\ >nul
copy /y USB101D_64.lib output\x64\ >nul

echo.
echo === ȫ��������ɣ��ļ�λ�� output\x86 �� output\x64 ===
pause
exit /b 0

:vcfail
echo [ERROR] vcvarsall ����ʧ�ܣ�δ��ȷ���ر��뻷����
pause
exit /b 1

:buildfail
echo [ERROR] ����ʧ�ܣ������Ϸ� cl �����
pause
exit /b 1 