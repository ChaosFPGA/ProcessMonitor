#include <windows.h>
#include <fstream>
#include <string>

// Original WriteFile function pointer
typedef BOOL (WINAPI *pWriteFile)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
pWriteFile originalWriteFile = NULL;

// Log file
std::ofstream logFile;

// Hooked WriteFile function
BOOL WINAPI HookedWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, 
                           LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
    // Log the call
    logFile.open("C:\\Temp\\ProcessMonitorLog.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << "WriteFile called with " << nNumberOfBytesToWrite << " bytes at " 
                << GetTickCount() << "ms" << std::endl;
        logFile.close();
    }

    // Call original function
    return originalWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

// Function to install the hook
BOOL InstallHook() {
    HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
    if (!hKernel32) return FALSE;

    void* originalAddr = (void*)GetProcAddress(hKernel32, "WriteFile");
    if (!originalAddr) return FALSE;

    originalWriteFile = (pWriteFile)originalAddr;

    BYTE jump[5] = { 0xE9 }; // JMP instruction
    DWORD oldProtect;
    VirtualProtect(originalAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    uintptr_t relativeAddr = (uintptr_t)HookedWriteFile - (uintptr_t)originalAddr - 5;
    memcpy(jump + 1, &relativeAddr, 4);

    memcpy(originalAddr, jump, 5);
    VirtualProtect(originalAddr, 5, oldProtect, &oldProtect);

    return TRUE;
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        if (!InstallHook()) {
            MessageBox(NULL, "Failed to install hook", "Error", MB_OK | MB_ICONERROR);
            return FALSE;
        }
        break;
    case DLL_PROCESS_DETACH:
        // Cleanup if needed
        break;
    }
    return TRUE;
}