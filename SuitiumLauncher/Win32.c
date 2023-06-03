#if _WIN32

#include <stdlib.h>
#include <Windows.h>

void abortWithError(const char *reason)
{
    MessageBox(NULL, reason, "Fatal Error", MB_OK);
    abort();
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    int argC = __argc;
    char **argV = __argv;

    const char *pathToGameFolder = ".";

    for (int argNum = 1; argNum < argC; argNum++)
    {
        pathToGameFolder = argV[argC]; // TODO: this is shit
    }

    // TODO: the actual thing

    return 0;
}

#endif