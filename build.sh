#!/bin/bash

build_type=$1
target=$2

if [ "$build_type" = "--debug" ]; then
    if [ "$target" = "engine" ]; then
        cmake --build build/debug/ --config Debug --target peria_engine
    elif [ "$target" = "app" ]; then 
        cmake --build build/debug/ --config Debug --target app
    elif [ "$target" = "all" ]; then 
        cmake --build build/debug/ --config Debug
    fi
elif [ "$build_type" = "--release" ]; then
    if [ "$target" = "engine" ]; then
        cmake --build build/release/ --config Release --target peria_engine
    elif [ "$target" = "app" ]; then 
        cmake --build build/release/ --config Release --target app
    elif [ "$target" = "all" ]; then 
        cmake --build build/release/ --config Release
    fi
else
    echo must specify build type
fi
