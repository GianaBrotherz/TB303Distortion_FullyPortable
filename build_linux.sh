#!/bin/bash
echo "🐧 Building TB303 Distortion for Linux..."

# Install dependencies
echo "📦 Installing dependencies..."
if command -v apt-get >/dev/null 2>&1; then
    sudo apt-get update
    sudo apt-get install -y build-essential libasound2-dev libjack-jackd2-dev \
                            libfreetype6-dev libx11-dev libxcomposite-dev \
                            libxcursor-dev libxext-dev libxinerama-dev \
                            libxrandr-dev libxrender-dev libwebkit2gtk-4.0-dev \
                            libglu1-mesa-dev mesa-common-dev
elif command -v yum >/dev/null 2>&1; then
    sudo yum groupinstall -y "Development Tools"
    sudo yum install -y alsa-lib-devel jack-audio-connection-kit-devel \
                        freetype-devel libX11-devel libXcomposite-devel \
                        libXcursor-devel libXext-devel libXinerama-devel \
                        libXrandr-devel libXrender-devel webkit2gtk3-devel \
                        mesa-libGLU-devel
fi

# Build
echo "🔨 Building with make..."
cd Builds/LinuxMakefile
make CONFIG=Release -j$(nproc)

echo "✅ Linux Build complete! VST3: build/TB303Distortion.vst3"
