#!/bin/zsh

cmake -B build/macos/arm64 \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_BUILD_TYPE=Release

cmake -B build/macos/x86_64 \
    -DCMAKE_OSX_ARCHITECTURES=x86_64 \
    -DCMAKE_BUILD_TYPE=Release

# Now, compile both of them
cmake --build build/macos/arm64
cmake --build build/macos/x86_64
