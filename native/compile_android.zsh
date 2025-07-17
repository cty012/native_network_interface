#!/bin/zsh

# NDK path
export NDK=~/Library/Android/sdk/ndk/29.0.13599879

# 1. Build for arm64-v8a (Modern phones)
cmake -B build/android/arm64-v8a \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-24 \
    -DCMAKE_BUILD_TYPE=Release

# 2. Build for armeabi-v7a (Older 32-bit phones)
cmake -B build/android/armeabi-v7a \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=armeabi-v7a \
    -DANDROID_PLATFORM=android-24 \
    -DCMAKE_BUILD_TYPE=Release

# 3. Build for x86_64 (Emulators on Intel/AMD, Chromebooks)
cmake -B build/android/x86_64 \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=x86_64 \
    -DANDROID_PLATFORM=android-24 \
    -DCMAKE_BUILD_TYPE=Release

# 4. Build for x86 (Older 32-bit emulators)
cmake -B build/android/x86 \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=x86 \
    -DANDROID_PLATFORM=android-24 \
    -DCMAKE_BUILD_TYPE=Release

# Now, compile all of them
cmake --build build/android/arm64-v8a
cmake --build build/android/armeabi-v7a
cmake --build build/android/x86_64
cmake --build build/android/x86
