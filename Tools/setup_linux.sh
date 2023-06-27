#! /bin/bash

if [ ! -d "Output" ]
then
    mkdir Output
fi

cd Output
mkdir moonjit
cp -r ../Modules/moonjit ./
cd moonjit
make -j4 BUILDMODE=static CFLAGS+="-fPIC"
cd ..
cd ..