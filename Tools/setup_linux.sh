#! /bin/bash

if [ ! -d "Output" ]
then
    mkdir Output
fi

cd Output
mkdir moonjit
cp -r ../Modules/moonjit ./
cd moonjit
make -j4
cd ..
cd ..

echo /////////////
echo If moonjit was compiled successfully, copy libluajit.so to your game folder
echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\