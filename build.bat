@echo off

set PROJUCER="Projucer.exe"

cmake -Bbuild -G "Visual Studio 17 2022"
cmake --build build --target rtstft_ctl_VST3 --config Release
mkdir installer
cd installer
echo D|xcopy /e /y "..\build\rtstft_ctl_artefacts\Release\VST3\rtstft_ctl.vst3\" rtstft_ctl.vst3
COPY ..\win_installer.iss .
iscc win_installer.iss
COPY Output\rtstft_ctl-windows.exe ..\rtstft_ctl-windows.exe
cd ..
rd /S /Q installer