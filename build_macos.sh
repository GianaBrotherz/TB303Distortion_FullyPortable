#!/bin/bash
echo "🍎 Building TB303 Distortion for macOS..."

# Regenerate Xcode project
echo "📋 Regenerating Xcode project..."
if command -v projucer >/dev/null 2>&1; then
    projucer --resave TB303Distortion_Portable.jucer
else
    echo "⚠️  Projucer not found. Please download JUCE and add Projucer to PATH"
    echo "   Or use: /path/to/Projucer.app/Contents/MacOS/Projucer --resave TB303Distortion_Portable.jucer"
    exit 1
fi

# Build with Xcode
cd Builds/MacOSX
xcodebuild -project TB303Distortion.xcodeproj \
           -scheme "TB303Distortion - All" \
           -configuration Release \
           clean build \
           ARCHS="arm64 x86_64" \
           VALID_ARCHS="arm64 x86_64"

echo "✅ macOS Build complete! VST3: build/Release/TB303Distortion.vst3"
