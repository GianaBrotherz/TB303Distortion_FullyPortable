@echo off
echo 🪟 Building TB303 Distortion for Windows...

REM Prüfe Visual Studio Installation
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
) else (
    echo ❌ Visual Studio 2022 not found!
    echo Please install Visual Studio 2022 Community (free)
    echo https://visualstudio.microsoft.com/downloads/
    pause
    exit /b 1
)

echo ✅ Visual Studio found: %MSBUILD%

REM Build Plugin
echo 🔨 Building VST3...
cd Builds\VisualStudio2022
%MSBUILD% TB303Distortion.sln /p:Configuration=Release /p:Platform=x64 /p:UseMultiToolTask=true /maxcpucount

if %ERRORLEVEL% EQU 0 (
    echo ✅ Build successful!
    echo 📁 VST3 Location: x64\Release\VST3\TB303Distortion.vst3
    echo.
    echo 📥 Installation:
    echo copy "x64\Release\VST3\TB303Distortion.vst3" "C:\Program Files\Common Files\VST3\"
    echo.
) else (
    echo ❌ Build failed!
    echo Check Visual Studio installation and Windows SDK
)

pause
