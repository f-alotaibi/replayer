rmdir -Force -Recurse "build/windows_package/"
mkdir build/windows_package/
cp -r external/obs-studio/build-windows/rundir/Debug build/windows_package/replayer
cp external/obs-studio/.deps/obs-deps-2025-07-11-x64/bin/*.dll build/windows_package/replayer/bin/64bit/
cp build/Debug/replayer.exe build/windows_package/replayer/bin/64bit/
windeployqt.exe build\windows_package\replayer\bin\64bit\replayer.exe