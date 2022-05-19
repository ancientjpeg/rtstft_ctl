@echo off

set PROJUCER="Projucer.exe"

"C:\\JUCE\"%PROJUCER% --resave rtstft_ctl.jucer
msbuild  "Builds\VisualStudio2022\rtstft_ctl.sln" -p:Configuration=Release
mkdir installer
cd installer
COPY "..\Builds\VisualStudio2022\x64\Release\VST3\rtstft_ctl.vst3" .
COPY ..\win_installer.iss .
iscc win_installer.iss
COPY Output\rtstft_ctl-windows.exe ..\rtstft_ctl-windows.exe
cd ..
rd /S /Q installer