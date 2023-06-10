@echo off

IF NOT EXIST "Output\" (
    mkdir Output
)

pushd Output
mkdir moonjit
xcopy ..\moonjit .\moonjit /E
pushd moonjit\src
start msvcbuild
popd
popd