#pragma once

#include <iostream>
#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ThreadProcA(LPVOID lpParameter);
DWORD WINAPI ThreadProcB(LPVOID lpParameter);
DWORD WINAPI ThreadProcC(LPVOID lpParameter);
void __cdecl OutputDebugStringF(const char* format, ...);
