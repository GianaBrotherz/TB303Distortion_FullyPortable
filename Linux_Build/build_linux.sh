#!/bin/bash
echo "Building TB303 Distortion for Linux..."

# Install dependencies
sudo apt update
sudo apt install -y build-essential libasound2-dev libjack-jackd2-dev \
                    libfreetype6-dev libx11-dev libxcomposite-dev \
                    libxcursor-dev libxext-dev libxinerama-dev \
                    libxrandr-dev libxrender-dev

# Build VST3
g++ -std=c++17 -O3 -shared -fPIC -DJUCE_VST3=1 \
    -I../Source -I../JUCE/modules -I../vst3sdk \
    ../Source/*.cpp \
    ../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_VST3.cpp \
    -o TB303Distortion.vst3

if [ -f "TB303Distortion.vst3" ]; then
    echo "✅ Linux VST3 erfolgreich erstellt!"
    echo "📤 Installiere in VST3 Ordner..."
    mkdir -p ~/.vst3
    cp TB303Distortion.vst3 ~/.vst3/
    echo "🎉 Installation abgeschlossen!"
else
    echo "❌ Build fehlgeschlagen."
fi
