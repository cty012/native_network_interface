#!/bin/zsh

# Clear previously installed libraries
rm -rf bin/ios/arm64
rm -rf bin/ios-sim/arm64
rm -rf bin/ios-sim/x86_64
rm -rf bin/ios-sim/universal

# Create directory structure
mkdir -p bin/ios/arm64
mkdir -p bin/ios-sim/arm64
mkdir -p bin/ios-sim/x86_64
mkdir -p bin/ios-sim/universal

# Move the dynamic libraries to the correct location
cp build/ios/arm64/libnetwork_interface.dylib bin/ios/arm64/

cp build/ios-sim/arm64/libnetwork_interface.dylib bin/ios-sim/arm64/

cp build/ios-sim/x86_64/libnetwork_interface.dylib bin/ios-sim/x86_64/

# Create a universal library for the simulator
lipo -create \
    bin/ios-sim/arm64/libnetwork_interface.dylib \
    bin/ios-sim/x86_64/libnetwork_interface.dylib \
    -output bin/ios-sim/universal/libnetwork_interface.dylib

# Copy the libraries to the Frameworks directory for CocoaPods
rm -rf ../ios/NetworkInterface.xcframework
xcodebuild -create-xcframework \
    -library bin/ios/arm64/libnetwork_interface.dylib \
    -library bin/ios-sim/universal/libnetwork_interface.dylib \
    -output ../ios/NetworkInterface.xcframework
