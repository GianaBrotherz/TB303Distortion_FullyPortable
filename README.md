# 🎵 TB303 Distortion - Cross-Platform VST3 Plugin

![Build Status](https://github.com/USERNAME/TB303-Distortion/workflows/Build%20TB303%20Distortion%20for%20All%20Platforms/badge.svg)

## 🎯 Universal Audio Plugin für alle Plattformen

**TB303 Distortion** ist ein Cross-Platform VST3 Audio Plugin mit animiertem Acid Smiley, der authentischen TB-303 Distortion Sound liefert.

## ✨ Features
- 🎨 **Animated Acid Smiley** - Morpht visuell mit dem Distortion Level
- 🔊 **TB-303 Style Distortion** - Authentischer Acid House Sound
- 🎛️ **MIDI Automation** - Alle Parameter über MIDI steuerbar
- 📦 **Embedded Resources** - Keine externen Dateien erforderlich
- 🌍 **Cross-Platform** - Läuft nativ auf Windows, macOS und Linux
- **Universal Binary** - Intel & Apple Silicon (macOS)
- **Statically Linked** - Keine externen Abhängigkeiten
- **Embedded Resources** - Smiley Sprite Sheets eingebettet

## 🚀 Quick Start

### macOS
```bash
chmod +x build_macos.sh install_macos.sh
./build_macos.sh
./install_macos.sh
```

### Windows
```cmd
build_windows.bat
install_windows.bat
```

### Linux
```bash
chmod +x build_linux.sh install_linux.sh
./build_linux.sh
./install_linux.sh
```

## 📦 Was ist enthalten

- **Source Code** - Vollständiger C++ Source
- **JUCE Modules** - Lokale Kopie für Portabilität
- **Resources** - Eingebettete Sprite Sheets
- **Build Scripts** - Für alle Plattformen
- **Install Scripts** - Automatische Installation

## 🔧 Anforderungen

### macOS
- Xcode Command Line Tools
- macOS 10.13+

### Windows  
- Visual Studio 2022 Community (kostenlos)
- Windows 10/11

### Linux
- GCC/Clang Compiler
- Standard Development Libraries (automatisch installiert)

## 🎨 Plugin Formats

- **VST3** - Alle Plattformen
- **AU** - macOS only  
- **Standalone** - Alle Plattformen

## ✅ Getestet auf

- macOS Big Sur - Sequoia (Intel & Apple Silicon)
- Windows 10/11 (x64)
- Ubuntu 20.04+ / Debian 11+
- Fedora 35+ / CentOS 8+

## 🐛 Problemlösung

**macOS Sicherheitswarnung:**
```bash
sudo xattr -r -d com.apple.quarantine ~/Library/Audio/Plug-Ins/VST3/TB303Distortion.vst3
```

**Windows Defender:**
- Plugin zu Ausnahmen hinzufügen

**Linux Dependencies fehlen:**
```bash
sudo apt install build-essential libasound2-dev libjack-jackd2-dev
```

## 🎵 Verwendung

1. Plugin in DAW laden
2. Smiley-Animation zeigt Distortion Level
3. Dry/Wet Parameter steuert Effektstärke
4. MIDI Input für parameter automation

**🎉 Viel Spaß mit TB303 Distortion!**
