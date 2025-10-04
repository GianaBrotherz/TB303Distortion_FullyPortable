#!/bin/bash
echo "📥 Installing TB303Distortion on macOS..."

if [ ! -d "Builds/MacOSX/build/Release/TB303Distortion.vst3" ]; then
    echo "❌ Plugin not built yet. Run ./build_macos.sh first"
    exit 1
fi

# Install VST3
mkdir -p "$HOME/Library/Audio/Plug-Ins/VST3"
cp -R "Builds/MacOSX/build/Release/TB303Distortion.vst3" "$HOME/Library/Audio/Plug-Ins/VST3/"

# Install AU
if [ -d "Builds/MacOSX/build/Release/TB303Distortion.component" ]; then
    mkdir -p "$HOME/Library/Audio/Plug-Ins/Components"
    cp -R "Builds/MacOSX/build/Release/TB303Distortion.component" "$HOME/Library/Audio/Plug-Ins/Components/"
fi

# Remove quarantine
xattr -r -d com.apple.quarantine "$HOME/Library/Audio/Plug-Ins/VST3/TB303Distortion.vst3" 2>/dev/null || true
xattr -r -d com.apple.quarantine "$HOME/Library/Audio/Plug-Ins/Components/TB303Distortion.component" 2>/dev/null || true

echo "✅ TB303Distortion installed successfully!"
