#!/bin/bash

build64(){
    cmake -Bbuild -G"Visual Studio 15 2017 Win64"
    cmake --build build --config Release
}

build32(){
    cmake -Bbuild32 -G"Visual Studio 15 2017"
    cmake --build build32 --config Release
}

# clean up old builds
rm -Rf build/
rm -Rf build32/
rm -Rf bin/Win64/
rm -Rf bin/Win32/

# cmake new builds
build64 &
build32 &
wait

# copy builds to bin
mkdir -p bin/Win64
mkdir -p bin/Win32
declare -a plugins=("BBDDelay")
for plugin in "${plugins[@]}"; do
    cp -R build/${plugin}_artefacts/Release/Standalone/${plugin}.exe bin/Win64/${plugin}.exe
    # cp -R build/${plugin}_artefacts/Release/VST/${plugin}.dll bin/Win64/${plugin}.dll
    cp -R build/${plugin}_artefacts/Release/VST3/${plugin}.vst3 bin/Win64/${plugin}.vst3

    cp -R build32/${plugin}_artefacts/Release/Standalone/${plugin}.exe bin/Win32/${plugin}.exe
    # cp -R build32/${plugin}_artefacts/Release/VST/${plugin}.dll bin/Win32/${plugin}.dll
    cp -R build32/${plugin}_artefacts/Release/VST3/${plugin}.vst3 bin/Win32/${plugin}.vst3
done
