#!/bin/zsh

# Clear previously installed libraries
rm -rf bin/macos/arm64
rm -rf bin/macos/x86_64
rm -rf bin/macos/universal

# Create directory structure
mkdir -p bin/macos/arm64
mkdir -p bin/macos/x86_64
mkdir -p bin/macos/universal

# Move the dynamic libraries to the correct location
cp build/macos/arm64/libnetwork_interface.dylib bin/macos/arm64/

cp build/macos/x86_64/libnetwork_interface.dylib bin/macos/x86_64/

# Create the universal library
lipo -create \
  bin/macos/arm64/libnetwork_interface.dylib \
  bin/macos/x86_64/libnetwork_interface.dylib \
  -output bin/macos/universal/libnetwork_interface.dylib

# Copy the universal library to the Frameworks directory for CocoaPods
rm -rf ../macos/Frameworks
mkdir -p ../macos/Frameworks
cp bin/macos/universal/libnetwork_interface.dylib ../macos/Frameworks/
