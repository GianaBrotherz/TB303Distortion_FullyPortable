@echo off
echo 📥 Installing TB303Distortion VST3 for Windows...

REM Prüfe ob VST3 existiert
if not exist "Builds\VisualStudio2022\x64\Release\VST3\TB303Distortion.vst3" (
    echo ❌ VST3 not found! Build plugin first with build_windows_local.bat
    pause
    exit /b 1
)

REM System Installation (requires admin)
echo Installing to system directory...
if exist "C:\Program Files\Common Files\VST3\" (
    copy "Builds\VisualStudio2022\x64\Release\VST3\TB303Distortion.vst3" "C:\Program Files\Common Files\VST3\" >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo ✅ Installed to: C:\Program Files\Common Files\VST3\TB303Distortion.vst3
    ) else (
        echo ⚠️  System install failed. Trying user directory...
        goto USER_INSTALL
    )
) else (
    echo ⚠️  System VST3 directory not found. Using user directory...
    goto USER_INSTALL
)
goto DONE

:USER_INSTALL
REM User Installation
if not exist "%USERPROFILE%\AppData\Local\Programs\Common\VST3\" (
    mkdir "%USERPROFILE%\AppData\Local\Programs\Common\VST3"
)
copy "Builds\VisualStudio2022\x64\Release\VST3\TB303Distortion.vst3" "%USERPROFILE%\AppData\Local\Programs\Common\VST3\"
if %ERRORLEVEL% EQU 0 (
    echo ✅ Installed to: %USERPROFILE%\AppData\Local\Programs\Common\VST3\TB303Distortion.vst3
) else (
    echo ❌ Installation failed!
    pause
    exit /b 1
)

:DONE
echo.
echo 🎵 TB303Distortion VST3 installed successfully!
echo.
echo 🚀 Next steps:
echo 1. Open Bitwig Studio
echo 2. Settings ^> Locations ^> VST3 Plugin Locations
echo 3. Add VST3 directory if not present
echo 4. Rescan plugins
echo 5. Find plugin: Browser ^> Effects ^> GianaBrotherz ^> TB303Distortion
echo.
echo 🎨 Features:
echo - Animated Acid Smiley (morphs with distortion)
echo - TB-303 Style Distortion Effect
echo - MIDI Parameter Automation
echo.
pause
