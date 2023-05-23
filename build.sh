#!/bin/bash

build_type=$1
target=$2
if [ "$build_type" = "--debug" ]; then
    echo Compiling Debug Build
    echo Compiling engine library
    cmake --build build/debug/ --config Debug --target engine
    echo Compiling target $target
    cmake --build build/debug/ --config Debug --target $target
elif [ "$build_type" = "--release" ]; then
    echo Compiling Release Build
    echo Compiling engine library
    cmake --build build/release/ --config Release --target engine
    echo Compiling target $target
    cmake --build build/release/ --config Release --target $target
else
    echo must specify build type
fi
