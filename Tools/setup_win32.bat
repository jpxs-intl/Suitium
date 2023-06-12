@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

IF NOT EXIST "Output\" (
    mkdir Output
)

pushd Output
mkdir moonjit
xcopy ..\moonjit .\moonjit /E
pushd moonjit\src
call msvcbuild
popd
popd

echo /////////////
echo If moonjit was compiled successfully, copy lua51.dll to your game folder
echo \\\\\\\\\\\\\