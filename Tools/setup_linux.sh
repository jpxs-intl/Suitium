#! /bin/bash

if [ ! -d "Output"] 
then
    mkdir Output
fi

pushd Output
mkdir moonjit
cp -r ../Modules/moonjit ./moonjit
pushd moonjit
make -j4
popd
popd

echo /////////////
echo If moonjit was compiled successfully, copy libluajit.so and liblua51.so to your game folder
echo \\\\\\\\\\\\\\\\\\\\\\\\\\