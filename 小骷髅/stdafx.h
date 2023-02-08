// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 在此处引用程序需要的其他头文件
#include<mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include<shellapi.h>

#include"..\DS\DS.h"
#include"Running.h"

#ifdef _M_X64
#pragma comment(lib,"Running_x64.lib")
#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\DS.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\DS.lib")
#endif

#else
#pragma comment(lib,"Running.lib")
#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\DS.lib")
#else
#pragma comment(lib,"..\\Release\\DS.lib")
#endif
#endif