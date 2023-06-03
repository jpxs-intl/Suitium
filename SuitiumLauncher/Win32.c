#if _WIN32

#include <stdlib.h>
#include <string.h>
#include <Windows.h>

void AbortWithError(const char *reason)
{
    MessageBox(NULL, reason, "Fatal Error", MB_OK);
    abort();
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    int argC = __argc;
    char **argV = __argv;

    const char *gamePath = "subrosa.exe";

    for (int argNum = 1; argNum < argC; argNum++)
    {
        gamePath = argV[argNum]; // TODO: this is shit
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
    {
        AbortWithError("Could not open process"); // TODO: Add better error description?
    }

    ResumeThread(processInfo.hThread); // We created a process using CREATE_SUSPENDED, remember? (Of course you do, if you don't, seek a doctor)

    WaitForSingleObject(processInfo.hProcess, INFINITE);

    return 0;
}

#endif