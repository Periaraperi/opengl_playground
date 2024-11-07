#!/bin/bash

build_type=$1
target=$2 # unused for now

if [ "$build_type" = "--debug" ]; then
    echo Compiling Debug app
    cmake --build build/debug/ --config Debug --target app
elif [ "$build_type" = "--release" ]; then
    echo Compiling Release app 
    cmake --build build/release/ --config Release --target app 
else
    echo must specify build type
fi
