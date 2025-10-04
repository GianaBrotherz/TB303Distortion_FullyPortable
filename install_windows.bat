@echo off
echo 📥 Installing TB303Distortion on Windows...

if not exist "Builds\VisualStudio2022\x64\Release\VST3\TB303Distortion.vst3" (
    echo ❌ Plugin not built yet. Run build_windows.bat first
    pause
    exit /b 1
)

echo Copying VST3 to system directory...
xcopy "Builds\VisualStudio2022\x64\Release\VST3\TB303Distortion.vst3" "C:\Program Files\Common Files\VST3\" /E /I /Y

echo ✅ TB303Distortion installed successfully!
pause
