#!/bin/bash
echo "📥 Installing TB303Distortion on Linux..."

if [ ! -d "Builds/LinuxMakefile/build/TB303Distortion.vst3" ]; then
    echo "❌ Plugin not built yet. Run ./build_linux.sh first"
    exit 1
fi

# Install VST3 (user directory)
mkdir -p "$HOME/.vst3"
cp -R "Builds/LinuxMakefile/build/TB303Distortion.vst3" "$HOME/.vst3/"

echo "✅ TB303Distortion installed successfully!"
echo "   Location: $HOME/.vst3/TB303Distortion.vst3"
