@echo off

IF NOT EXIST "Output\" (
    mkdir Output
)

pushd Output
cmake -G "Visual Studio 17 2022" ..
cmake --build .
popd