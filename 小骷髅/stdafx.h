// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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