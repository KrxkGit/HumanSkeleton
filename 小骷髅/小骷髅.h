#pragma once

#include "resource.h"

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK PassWordProc(HWND, UINT, WPARAM, LPARAM);
VOID OnCommand(HWND,WPARAM,LPARAM);
VOID OnInitMainDlg(HWND);
BOOL RunByWindow(BOOL bRun);//��������