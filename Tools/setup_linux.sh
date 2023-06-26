#! /bin/bash

if [ ! -d "Output" ]
then
    mkdir Output
fi

pushd Output

popd

echo /////////////
echo If moonjit was compiled successfully, copy libluajit.so and liblua51.so to your game folder
echo \\\\\\\\\\\\\\\\\\\\\\\\\\