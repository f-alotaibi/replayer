rm -rf ./obs-studio/build-linux/

flags=(
    # Core
    "-DENABLE_UI=OFF"
    "-DENABLE_SCRIPTING=OFF"
    "-DENABLE_FRONTEND=OFF"

    # Plugins
    "-DENABLE_AJA=OFF"
    "-DENABLE_DECKLINK=OFF"
    "-DENABLE_SPEEXDSP=OFF"
    "-DENABLE_BROWSER=OFF"
    "-DENABLE_VLC=OFF"
    "-DENABLE_VST=OFF"
    "-DENABLE_WEBSOCKET=OFF"
    "-DENABLE_WEBRTC=OFF"
    "-DENABLE_RNNOISE=OFF"
    "-DENABLE_FREETYPE=OFF"

    # Linux
    # Plugins
    "-DENABLE_PIPEWIRE=OFF"
    "-DENABLE_V4L2=OFF"
    "-DENABLE_SNDIO=OFF"
    "-DENABLE_ALSA=OFF"

    # Features
    "-DENABLE_WAYLAND=OFF"
)

cmake -S ./obs-studio/ -B ./obs-studio/build-linux/ "${flags[@]}"
cmake --build ./obs-studio/build-linux/