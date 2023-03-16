#include "main.h"

HWND edit[7];
HANDLE hThread[7];
TCHAR editBuffer[7][100] = { 0 };
DWORD index[9] = { 0 };
HANDLE g_hEvent;
HANDLE g_hEvent1;
HANDLE hSemaphore;
/*
	因为ThreadProcB时,1,2的index已经改变了,但是ThreadProcC的时候又需要从0开始,所以用index[7],index[8]替代.
*/


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) //WinMain批注不一致,加上_In_
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProc);   //x64
	return 0;
}

LRESULT CALLBACK DialogProc(HWND hwndDlgMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case  WM_INITDIALOG:

		edit[0] = GetDlgItem(hwndDlgMain, IDC_EDIT_0);
		edit[1] = GetDlgItem(hwndDlgMain, IDC_EDIT_1);
		edit[2] = GetDlgItem(hwndDlgMain, IDC_EDIT_2);
		edit[3] = GetDlgItem(hwndDlgMain, IDC_EDIT_3);
		edit[4]= GetDlgItem(hwndDlgMain, IDC_EDIT_4);
		edit[5]= GetDlgItem(hwndDlgMain, IDC_EDIT_5);
		edit[6] = GetDlgItem(hwndDlgMain, IDC_EDIT_6);

		return TRUE;

	case  WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_START:

			hThread[0] = ::CreateThread(NULL, 0, ThreadProcA, NULL, 0, NULL);

			return TRUE;
		}
		break;

	case WM_CLOSE:
		EndDialog(hwndDlgMain, 0);
		return TRUE;
	}
	return FALSE;
}

DWORD WINAPI ThreadProcA(LPVOID lpParameter)
{
	//GetWindowText(edit[0], editBuffer[0], 10);
	//if (editBuffer[0][0] == NULL)
	//{
	//	MessageBox(0, 0, 0, 0);
	//}
	
	hSemaphore = CreateSemaphore(NULL, 0, 4, NULL);
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hEvent1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	hThread[1] = ::CreateThread(NULL, 0, ThreadProcB, (void*)1, 0, NULL);
	hThread[2] = ::CreateThread(NULL, 0, ThreadProcB, (void*)2, 0, NULL);
	hThread[3] = ::CreateThread(NULL, 0, ThreadProcC, (void*)3, 0, NULL);
	hThread[4] = ::CreateThread(NULL, 0, ThreadProcC, (void*)4, 0, NULL);
	hThread[5] = ::CreateThread(NULL, 0, ThreadProcC, (void*)5, 0, NULL);
	hThread[6] = ::CreateThread(NULL, 0, ThreadProcC, (void*)6, 0, NULL);
	
	SetEvent(g_hEvent1);
	SetEvent(g_hEvent);
	ReleaseSemaphore(hSemaphore, 2, NULL);
	//CloseHandle(hThread[1]);
	//CloseHandle(hThread[2]);
	return 0;
}

DWORD WINAPI ThreadProcB(LPVOID lpParameter)
{
	DWORD i = (DWORD)lpParameter;
	DWORD k = 0;
	while (true)
	{
		WaitForSingleObject(g_hEvent, INFINITE);
		GetWindowText(edit[0], editBuffer[0], 100);
		if (editBuffer[0][index[0]] == 0)
		{
			return 0;
		}
		editBuffer[i][index[i]] = editBuffer[0][index[0]];
		SetWindowText(edit[i], editBuffer[i]);
		index[0]++;
		index[i]++;
		SetEvent(g_hEvent);
		Sleep(100);
	}
	return 0;
}

DWORD WINAPI ThreadProcC(LPVOID lpParameter)
{
	DWORD i = (DWORD)lpParameter;
	while (true)
	{
		WaitForSingleObject(g_hEvent, INFINITE);
		WaitForSingleObject(hSemaphore, INFINITE);
		
		GetWindowText(edit[1], editBuffer[1], 100);
		if (editBuffer[1][index[7]] == 0)
		{
			GetWindowText(edit[2], editBuffer[2], 100);
			
			if (editBuffer[2][index[8]] == 0)
			{
				ReleaseSemaphore(hSemaphore, 1, NULL);
				SetEvent(g_hEvent);
				Sleep(100);
				continue;
			}
			editBuffer[i][index[i]] = editBuffer[2][index[8]];
			SetWindowText(edit[i], editBuffer[i]);
			index[8]++;
			index[i]++;
		}
		else
		{
			editBuffer[i][index[i]] = editBuffer[1][index[7]];
			SetWindowText(edit[i], editBuffer[i]);
			index[7]++;
			index[i]++;
		}
		ReleaseSemaphore(hSemaphore, 1, NULL);
		SetEvent(g_hEvent);
		Sleep(100);
	}
	return 0;
}

void __cdecl OutputDebugStringF(const char* format, ...)
{
	va_list vlArgs;
	char* strBuffer = (char*)GlobalAlloc(GPTR, 4096);

	va_start(vlArgs, format);
	_vsnprintf(strBuffer, 4096 - 1, format, vlArgs);
	va_end(vlArgs);
	strcat(strBuffer, "\n");
	OutputDebugStringA(strBuffer);
	GlobalFree(strBuffer);
	return;
}