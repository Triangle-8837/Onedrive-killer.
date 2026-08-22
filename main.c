#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 250
#define REG_KEY_PATH "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define ONEDRIVE_REG_NAME "OneDrive"
#define ONEDRIVE_EXE_PATH "%LOCALAPPDATA%\\Microsoft\\OneDrive\\OneDrive.exe"

HWND hStatusText;
HWND hToggleBtn;
BOOL isOneDriveEnabled = FALSE;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CheckOneDriveStatus();
void ToggleOneDrive(BOOL enable);
void KillOneDriveProcess();
void UpdateUI();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "OneDriveControlClass";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window registration failed", "Error", MB_ICONERROR);
        return 0;
    }

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "OneDrive Controller",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            hStatusText = CreateWindow(
                "STATIC", "Checking status...",
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                20, 30, 360, 30,
                hwnd, NULL, NULL, NULL
            );
            
            CreateWindow(
                "STATIC", "Current OneDrive Startup Status:",
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                20, 70, 360, 20,
                hwnd, NULL, NULL, NULL
            );

            hToggleBtn = CreateWindow(
                "BUTTON", "Loading...",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                100, 110, 200, 40,
                hwnd, (HMENU)1, NULL, NULL
            );

            CreateWindow(
                "BUTTON", "Force Kill Process",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                100, 160, 200, 30,
                hwnd, (HMENU)2, NULL, NULL
            );

            CheckOneDriveStatus();
            break;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) {
                ToggleOneDrive(!isOneDriveEnabled);
            } else if (LOWORD(wParam) == 2) {
                KillOneDriveProcess();
                MessageBox(hwnd, "OneDrive process has been terminated.", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void CheckOneDriveStatus() {
    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, REG_KEY_PATH, 0, KEY_READ, &hKey);
    
    isOneDriveEnabled = FALSE;
    if (result == ERROR_SUCCESS) {
        char valueBuf = {0};
        DWORD size = sizeof(valueBuf);
        DWORD type;
        if (RegQueryValueEx(hKey, ONEDRIVE_REG_NAME, NULL, &type, (LPBYTE)valueBuf, &size) == ERROR_SUCCESS) {
            isOneDriveEnabled = TRUE;
        }
        RegCloseKey(hKey);
    }
    UpdateUI();
}

void ToggleOneDrive(BOOL enable) {
    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, REG_KEY_PATH, 0, KEY_SET_VALUE, &hKey);
    
    if (result != ERROR_SUCCESS) {
        MessageBox(NULL, "Cannot open registry key, please run as administrator.", "Error", MB_ICONERROR);
        return;
    }

    if (enable) {
        char pathBuf[MAX_PATH] = {0};
        ExpandEnvironmentStrings(ONEDRIVE_EXE_PATH, pathBuf, sizeof(pathBuf));
        RegSetValueEx(hKey, ONEDRIVE_REG_NAME, 0, REG_SZ, (const BYTE*)pathBuf, strlen(pathBuf) + 1);
    } else {
        RegDeleteValue(hKey, ONEDRIVE_REG_NAME);
    }
    
    RegCloseKey(hKey);
    isOneDriveEnabled = enable;
    UpdateUI();
}

void KillOneDriveProcess() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, "OneDrive.exe") == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProcess) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);
}

void UpdateUI() {
    if (isOneDriveEnabled) {
        SetWindowText(hStatusText, "Status: Enabled (Auto Startup)");
        SetWindowText(hToggleBtn, "Disable OneDrive Startup");
    } else {
        SetWindowText(hStatusText, "Status: Disabled (No Auto Startup)");
        SetWindowText(hToggleBtn, "Enable OneDrive Startup");
    }
}