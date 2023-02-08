// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

HINSTANCE hInst;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if(ul_reason_for_call==DLL_PROCESS_ATTACH) {
		hInst=hModule;
	}
	return TRUE;
}

