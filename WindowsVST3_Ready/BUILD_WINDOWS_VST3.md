# 🪟 TB303 Distortion - Windows VST3 Build Anweisungen

## ⚡ Quick Setup (Windows PC)

### 1. Vorbereitung
```cmd
# Visual Studio 2022 Community installieren (kostenlos)
# https://visualstudio.microsoft.com/downloads/
# Workload: "Desktop development with C++"
```

### 2. Build
```cmd
# 1. Ordner TB303Distortion_FullyPortable auf Windows PC kopieren
# 2. PowerShell/CMD öffnen
cd TB303Distortion_FullyPortable\Builds\VisualStudio2022

# 3. Build mit MSBuild
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" TB303Distortion.sln /p:Configuration=Release /p:Platform=x64

# ODER: Visual Studio öffnen und TB303Distortion.sln bauen
```

### 3. VST3 Datei Location
```cmd
# Nach dem Build:
Builds\VisualStudio2022\x64\Release\VST3\TB303Distortion.vst3

# Das ist eine ECHTE .vst3 DATEI (nicht Ordner wie auf macOS)
```

### 4. Installation für Bitwig
```cmd
# VST3 nach System-Verzeichnis kopieren:
copy "x64\Release\VST3\TB303Distortion.vst3" "C:\Program Files\Common Files\VST3\"

# Oder Benutzer-Verzeichnis:
copy "x64\Release\VST3\TB303Distortion.vst3" "%USERPROFILE%\AppData\Local\Programs\Common\VST3\"
```

## 🔧 Troubleshooting

**Fehler: "MSBuild not found"**
```cmd
# Visual Studio Command Prompt verwenden:
# Start Menu > Visual Studio 2022 > Developer Command Prompt
# Dann: msbuild TB303Distortion.sln /p:Configuration=Release /p:Platform=x64
```

**Fehler: "Windows SDK missing"**
```cmd
# Visual Studio Installer öffnen
# Modify > Individual Components
# Neueste "Windows 10/11 SDK" auswählen
```

**Bitwig erkennt Plugin nicht**
```cmd
# 1. Bitwig > Settings > Locations > VST3 Plugin Locations
# 2. Pfad hinzufügen: C:\Program Files\Common Files\VST3
# 3. Rescan Plugins
```

## ✅ Ergebnis
- **TB303Distortion.vst3** - Echte Windows VST3 Datei
- **Smiley Animation** - Eingebettete Sprites funktionieren
- **Universal** - Läuft auf allen Windows 10/11 Systemen
- **No Dependencies** - Statically linked, keine DLLs erforderlich

🎵 **Happy Music Making!**
