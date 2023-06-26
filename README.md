# Suitium and SuitiumLauncher

This project contains multiple modules... \
Please check "Suitium" and "SuitiumLauncher" folders.

## Compiling & Testing (For Windows)

You will need VS2022 (Full or Build Tools) and CMake.

In PowerShell, run the following command ONCE to setup the environment:

```bat
./Tools/setup_win32.bat
```

If moonjit was compiled successfully, copy lua51.dll compiled by CMake to your game folder.
Then run the following command to compile and test Suitium:

```bat
./Tools/cmake_win32.bat ; ./Tools/test_win32.bat "C:\Program Files (x86)\Steam\steamapps\common\Sub Rosa\subrosa.exe" "PATH TO SUITIUM DLL HERE"
```

Replace "PATH TO SUITIUM DLL HERE" with the path to the .dll compiled by CMake.

Happy modding!