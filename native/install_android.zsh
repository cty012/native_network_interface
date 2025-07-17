#!/bin/zsh

# Clear previously installed libraries
rm -rf bin/android/arm64-v8a
rm -rf bin/android/armeabi-v7a
rm -rf bin/android/x86_64
rm -rf bin/android/x86

# Create directory structure
mkdir -p bin/android/arm64-v8a
mkdir -p bin/android/armeabi-v7a
mkdir -p bin/android/x86_64
mkdir -p bin/android/x86

# Move the dynamic libraries to the correct location
mv build/android/arm64-v8a/libnetwork_interface.so \
    bin/android/arm64-v8a/libnetwork_interface.so

mv build/android/armeabi-v7a/libnetwork_interface.so \
    bin/android/armeabi-v7a/libnetwork_interface.so

mv build/android/x86_64/libnetwork_interface.so \
    bin/android/x86_64/libnetwork_interface.so

mv build/android/x86/libnetwork_interface.so \
    bin/android/x86/libnetwork_interface.so
