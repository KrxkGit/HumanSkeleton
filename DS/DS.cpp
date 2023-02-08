// DS.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include"resource.h"
#include"DS.h"

BOOL bFirst=TRUE;
#pragma data_seg("Shared")
HHOOK hHook=NULL;
DWORD dwThreadId=0;
HWND hDialog=NULL;
HWND hDesktop=NULL;
#pragma data_seg()
#pragma comment(linker,"/SECTION:Shared,RWS")

extern HINSTANCE hInst;

VOID DoSetDestopIcon()
{
	RECT rc;
	GetWindowRect(hDesktop,&rc);
	
	DWORD dwStyle=GetWindowStyle(hDesktop);
	if(dwStyle & LVS_AUTOARRANGE)
		SetWindowLong(hDesktop, GWL_STYLE, dwStyle & ~LVS_AUTOARRANGE);

	int MaxCount=ListView_GetItemCount(hDesktop);
	for(int nItem=0;nItem<MaxCount;nItem++) {
		ListView_SetItemPosition(hDesktop,nItem,rand()%rc.right,rand()%rc.bottom);
	}
	SetWindowLong(hDesktop,GWL_STYLE,dwStyle);
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_APP:
		DoSetDestopIcon();
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg,WM_CLOSE);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK GetMsgProc(int code,WPARAM wParam,LPARAM lParam)
{
	if(bFirst) {
		bFirst=FALSE;
		hDialog=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DS),NULL,DlgProc);
		PostThreadMessage(dwThreadId,WM_NULL,0,0);//激活原线程

	//绘制桌面背景
	LVBKIMAGE lv={0};
	lv.ulFlags=LVBKIF_SOURCE_HBITMAP | LVBKIF_STYLE_TILE;
	lv.hbm=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));
	ListView_SetBkImage(hDesktop,&lv);
	}
	return CallNextHookEx(hHook,code,wParam,lParam);
}

VOID SetHook()
{
	if(hHook==NULL) {
		hDesktop=GetFirstChild(GetFirstChild(FindWindow(_T("ProgMan"),NULL)));
		dwThreadId=GetWindowThreadProcessId(hDesktop,NULL);
		hHook=SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,hInst,dwThreadId);
		DWORD dwTemp=dwThreadId;
		dwThreadId=GetCurrentThreadId();//等待隐形对话框创建完成
		PostThreadMessage(dwTemp,WM_NULL,0,0);//注入DLL
		MSG msg;
		GetMessage(&msg,NULL,NULL,NULL);
		//隐形对话框创建完毕，做你想做的
	}
	else {
		UnhookWindowsHookEx(hHook);
		hHook=NULL;
	}
}

VOID SendMyMessage(UINT message)
{
	SendMessage(hDialog,message,0,0);
}