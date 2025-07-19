rmdir -Force -Recurse "./build/"
cmake -S . -B "build/"
cmake --build "./build/"