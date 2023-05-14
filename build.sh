#!/bin/bash

build_type=$1
if [ "$build_type" = --debug ]; then
    echo Compiling Debug Build
    cmake --build build/debug/ --config Debug
elif [ "$build_type" = "--release" ]; then
    echo Compiling Release Build
    cmake --build build/release/ --config Release
else
    echo must specify build type
fi
