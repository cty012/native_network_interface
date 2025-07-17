#!/bin/zsh

# Clear previously installed libraries
rm -rf bin/ios/arm64
rm -rf bin/ios-sim/arm64
rm -rf bin/ios-sim/x86_64

# Create directory structure
mkdir -p bin/ios/arm64
mkdir -p bin/ios-sim/arm64
mkdir -p bin/ios-sim/x86_64

# Move the dynamic libraries to the correct location
mv build/ios/arm64/libnetwork_interface.dylib \
    bin/ios/arm64/libnetwork_interface.dylib

mv build/ios-sim/arm64/libnetwork_interface.dylib \
    bin/ios-sim/arm64/libnetwork_interface.dylib

mv build/ios-sim/x86_64/libnetwork_interface.dylib \
    bin/ios-sim/x86_64/libnetwork_interface.dylib
