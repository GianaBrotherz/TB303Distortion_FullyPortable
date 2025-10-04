@echo off
echo Building TB303 Distortion for Windows...

REM Check if Visual Studio is installed
where cl.exe >nul 2>nul
if %errorlevel% neq 0 (
    echo Visual Studio nicht gefunden. Installiere Visual Studio Community 2022.
    echo Download: https://visualstudio.microsoft.com/de/vs/community/
    pause
    exit /b 1
)

REM Build VST3
cl.exe /std:c++17 /O2 /DNDEBUG /DJUCE_VST3=1 /I../Source /I../JUCE/modules /I../vst3sdk ^
    ../Source/*.cpp ^
    ../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_VST3.cpp ^
    /link /DLL /OUT:TB303Distortion.vst3

if exist TB303Distortion.vst3 (
    echo ✅ Windows VST3 erfolgreich erstellt!
    echo 📁 Datei: %CD%\TB303Distortion.vst3
    echo 📤 Kopiere nach VST3 Ordner...
    copy TB303Distortion.vst3 "%PROGRAMFILES%\Common Files\VST3\"
    echo 🎉 Installation abgeschlossen!
) else (
    echo ❌ Build fehlgeschlagen.
)

pause
