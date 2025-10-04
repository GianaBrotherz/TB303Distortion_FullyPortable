#!/bin/bash
echo "Building TB303 Distortion for macOS..."

# Check if Xcode is installed
if ! command -v xcodebuild &> /dev/null; then
    echo "❌ Xcode nicht installiert. Installiere Xcode Command Line Tools:"
    echo "xcode-select --install"
    exit 1
fi

# Build VST3 Bundle
mkdir -p TB303Distortion.vst3/Contents/MacOS
mkdir -p TB303Distortion.vst3/Contents/Resources

# Compile
g++ -std=c++17 -O3 -bundle -DJUCE_VST3=1 \
    -I../Source -I../JUCE/modules -I../vst3sdk \
    ../Source/*.cpp \
    ../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_VST3.cpp \
    -framework CoreFoundation -framework CoreAudio -framework AudioUnit \
    -o TB303Distortion.vst3/Contents/MacOS/TB303Distortion

# Create Info.plist
cat > TB303Distortion.vst3/Contents/Info.plist << 'PLIST'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>TB303Distortion</string>
    <key>CFBundleIdentifier</key>
    <string>com.tb303.distortion</string>
    <key>CFBundleName</key>
    <string>TB303Distortion</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
</dict>
</plist>
PLIST

if [ -f "TB303Distortion.vst3/Contents/MacOS/TB303Distortion" ]; then
    echo "✅ macOS VST3 erfolgreich erstellt!"
    echo "📤 Installiere in VST3 Ordner..."
    cp -r TB303Distortion.vst3 ~/Library/Audio/Plug-Ins/VST3/
    echo "🎉 Installation abgeschlossen!"
else
    echo "❌ Build fehlgeschlagen."
fi
