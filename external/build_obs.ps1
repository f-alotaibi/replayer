$OBSDir = Resolve-Path "obs-studio"
rmdir ./build-windows/
Set-Location $OBSDir

$Flags = @(
    "-DENABLE_UI=OFF",
    "-DENABLE_SCRIPTING=OFF",
    "-DENABLE_FRONTEND=OFF",

    "-DENABLE_AJA=OFF",
    "-DENABLE_SCRIPTING=OFF",
    "-DENABLE_DECKLINK=OFF",
    "-DENABLE_VIRTUALCAM=OFF",
    "-DENABLE_BROWSER=OFF",
    "-DENABLE_VLC=OFF",
    "-DENABLE_VST=OFF",
    "-DENABLE_WEBSOCKET=OFF",
    "-DENABLE_WEBRTC=OFF",
    "-DENABLE_RNNOISE=OFF",
    "-DENABLE_SPEEXDSP=OFF"
)

cmake -S . -B ./build-windows/ @Flags
cmake --build ./build-windows/

