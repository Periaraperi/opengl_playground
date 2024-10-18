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

#if [ "$build_type" = "--debug" ]; then
#    if ["$target" = "app"]; then
#        echo Compiling Debug $target
#        cmake --build build/debug/ --config Debug --target $target
#    else
#        echo Compiling Debug Build
#        echo Compiling engine library
#        cmake --build build/debug/ --config Debug --target engine
#        echo Compiling target $target
#        cmake --build build/debug/ --config Debug --target $target
#    fi
#elif [ "$build_type" = "--release" ]; then
#    if ["$target" = "app"]; then
#        echo Compiling Release $target
#        cmake --build build/release/ --config Release --target $target
#    else
#        echo Compiling Release Build
#        echo Compiling engine library
#        cmake --build build/release/ --config Release --target engine
#        echo Compiling target $target
#        cmake --build build/release/ --config Release --target $target
#    fi
#else
#    echo must specify build type
#fi
