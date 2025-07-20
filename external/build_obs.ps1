rmdir -Force -Recurse ./obs-studio/build-windows/

$Flags = @(
    # Core
    "-DENABLE_UI=OFF",
    "-DENABLE_SCRIPTING=OFF",
    "-DENABLE_FRONTEND=OFF",

    # Plugins
    "-DENABLE_AJA=OFF",
    "-DENABLE_DECKLINK=OFF",
    "-DENABLE_BROWSER=OFF",
    "-DENABLE_VLC=OFF",
    "-DENABLE_VST=OFF",
    "-DENABLE_WEBSOCKET=OFF",
    "-DENABLE_WEBRTC=OFF",
    "-DENABLE_RNNOISE=OFF",
    "-DENABLE_SPEEXDSP=OFF",
    "-DENABLE_FREETYPE=OFF",

    # Windows
    # Plugins
    "-DENABLE_COREAUDIO_ENCODER=OFF",

    # Features
    "-DENABLE_VIRTUALCAM=OFF",
    "-DENABLE_NVAFX=OFF",
    "-DENABLE_NVVFX=OFF"
)

cmake -S ./obs-studio/ -B ./obs-studio/build-windows/ @Flags
cmake --build ./obs-studio/build-windows/