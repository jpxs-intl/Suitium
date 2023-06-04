#! /bin/bash

if [ ! -d "Output"] 
then
    mkdir Output
fi

cd Output

cmake ..
cmake --build .

cd ..
