@echo off
echo 🪟 Building TB303 Distortion for Windows...

echo 📋 Regenerating Visual Studio project...
if exist "C:\Program Files\ROLI\JUCE\Projucer.exe" (
    "C:\Program Files\ROLI\JUCE\Projucer.exe" --resave TB303Distortion_Portable.jucer
) else (
    echo ⚠️  Projucer not found. Please install JUCE and adjust path
    echo    Or manually run: Projucer.exe --resave TB303Distortion_Portable.jucer
    pause
    exit /b 1
)

echo 🔨 Building with Visual Studio...
cd Builds\VisualStudio2022
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" TB303Distortion.sln /p:Configuration=Release /p:Platform=x64
) else (
    echo ⚠️  MSBuild not found. Please install Visual Studio 2022
    echo    Or build manually with Visual Studio IDE
    pause
    exit /b 1
)

echo ✅ Windows Build complete! VST3: Builds\VisualStudio2022\x64\Release\VST3\TB303Distortion.vst3
pause
