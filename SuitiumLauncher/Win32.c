#if _WIN32

#include <stdlib.h>
#include <string.h>
#include <Windows.h>

void AbortWithError(const char *reason)
{
    MessageBox(NULL, reason, "Fatal Error", MB_OK | MB_ICONERROR);
    abort();
}

void InjectDLL(HANDLE process, const char *dllPath)
{
    size_t dllPathLength = strlen(dllPath);

    FARPROC loadLibraryProc = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA");
    if (loadLibraryProc == NULL)
        AbortWithError("Could not locate LoadLibraryA");

    LPVOID remoteDllPath = VirtualAllocEx(process, NULL, dllPathLength + 1, MEM_COMMIT, PAGE_READWRITE); // Allocate DLL path in the target process
    if (remoteDllPath == NULL)
        AbortWithError("Could not allocate remote DLL path");
    WriteProcessMemory(process, remoteDllPath, dllPath, dllPathLength + 1, NULL);

    HANDLE remoteThread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryProc, remoteDllPath, 0, NULL); // Load the DLL and execute the entry point
    if (remoteThread == NULL)
        AbortWithError("Could not create remote thread");

    WaitForSingleObject(remoteThread, INFINITE);

    DWORD exitCode; // This will be 0 (NULL) if LoadLibraryA fails
    GetExitCodeThread(remoteThread, &exitCode);
    if (exitCode == 0)
        AbortWithError("LoadLibraryA failed");

    VirtualFreeEx(process, remoteDllPath, 0, MEM_RELEASE);
    CloseHandle(remoteThread);
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    (void)instance; (void)prevInstance; (void)cmdLine; (void)cmdShow;

    int argC = __argc;
    char **argV = __argv;

    const char *gamePath = "subrosa.exe";
    const char *dllPath = "Suitium.dll";

    for (int argNum = 1; argNum < argC; argNum++) // TODO: this is shit
    {
        if (strcmp(argV[argNum], "--help") == 0)
        {
            MessageBox(NULL, "SuitiumLauncher.exe [gamePath=\"subrosa.exe\"] [dllPath=\"Suitium.dll\"]", "Help", MB_OK | MB_ICONINFORMATION);
            return 0;
        }

        if (argNum == 1)
            gamePath = argV[argNum];
        else if (argNum == 2)
            dllPath = argV[argNum];
        else
            AbortWithError("Too much args, try --help");
    }

    size_t gameDirectorySize = strlen(gamePath);
    char *gameDirectory = malloc(gameDirectorySize + 1);

    memcpy(gameDirectory, gamePath, gameDirectorySize);
    gameDirectory[gameDirectorySize] = '\0';

    // We need to get the game directory, I'm simply gonna get the "up" folder for now
    while (gameDirectorySize > 0)
    {
        if (gameDirectory[gameDirectorySize] == '\\' || gameDirectory[gameDirectorySize] == '/')
            break;

        gameDirectory[gameDirectorySize] = '\0';
        gameDirectorySize--;
    }
    
    realloc(gameDirectory, gameDirectorySize + 1);
    gameDirectory[gameDirectorySize] = '\0';

    STARTUPINFO startupInfo;
    ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    PROCESS_INFORMATION processInfo;
    //ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

    BOOL success = CreateProcess(
        gamePath, NULL,
        NULL, NULL,
        FALSE,
        CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS, // We gotta create a suspended process so we can inject Suitium.dll before anything runs
        NULL, gameDirectorySize > 0 ? gameDirectory : NULL,
        &startupInfo, &processInfo
    );
    if (!success)
        AbortWithError("Could not open process"); // TODO: Add better error description?

    InjectDLL(processInfo.hProcess, dllPath);
    ResumeThread(processInfo.hThread); // We created a process using CREATE_SUSPENDED, remember? (Of course you do, if you don't, seek a doctor)

    WaitForSingleObject(processInfo.hProcess, INFINITE);

    return 0;
}

#endif