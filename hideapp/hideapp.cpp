// hideapp.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include <tchar.h>

void __declspec(noreturn) handle_error(LPCSTR lpErrorMessage)
{
    MessageBoxA(NULL, lpErrorMessage, "Error", MB_ICONEXCLAMATION);
    ExitProcess(GetLastError());
}

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow)
{
    STARTUPINFOA startupInfo;
    SecureZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    startupInfo.cb = sizeof(STARTUPINFO);

    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    PROCESS_INFORMATION procInfo;

    BOOL result = CreateProcessA(NULL, lpCmdLine,
                                 NULL, NULL, TRUE, // Handle inheritance
                                 NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
                                 NULL, NULL, &startupInfo, &procInfo);

    if (!result)
    {
        CHAR lpErrorFormat[] =
                  "Failed to start new process.\n\n"
                  "Cmd:\n%s\n\n"
                  "Error: %d";

        LPSTR lpErrorMessage = new CHAR[_countof(lpErrorFormat) + strlen(lpCmdLine) + 64];
        wsprintfA(lpErrorMessage, lpErrorFormat, lpCmdLine, GetLastError());
        handle_error(lpErrorMessage);
    }

    WaitForSingleObject(procInfo.hProcess, INFINITE);

    DWORD dwExitCode;
    if (!GetExitCodeProcess(procInfo.hProcess, &dwExitCode))
        dwExitCode = 0;

    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);

    ExitProcess(dwExitCode);
}
