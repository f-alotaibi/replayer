# Replayer

Replay buffer using OBS's libobs

# Requirements

CMake

A C++ compiler (MSVC For windows, GCC for linux)

All requirements from OBS [Link](https://github.com/obsproject/obs-studio/wiki/Building-OBS-Studio)

All requirements from libuiohook [Link](https://github.com/kwhat/libuiohook)

# Building

```
git clone --recursive https://github.com/f-alotaibi/replayer
cd replayer/
```

Before you build, you'll need to build OBS

```
cd external/
./build_obs.sh (linux) OR ./build_obs.ps1 (windows)
```

Then build `replayer`

```
cd ../
cmake -S . -B "build/"
cmake --build ./build/
```

You'll need to manually copy `libobs` `data` from obs

# TODO:

- [x] Build scripts (`build-scripts`)
- [ ] Automatic packaging
- [x] Linux: Avoid Conflicts with OBS (packaged in /opt/replayer)
- [x] Configuration in a file (Default [QSettings](https://doc.qt.io/qt-6/qsettings.html#platform-specific-notes) behaviour)
- [x] Figure out why `libobs-d3d11` doesn't work with Replayer (fixed by an obs update?)
- [x] Automatically find display resolution in X11
