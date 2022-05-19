[Setup]
AppName=rtstft_ctl
ArchitecturesInstallIn64BitMode=x64
AppVersion=0.1.0
DefaultDirName={commoncf}
DefaultGroupName=rtstft_ctl
OutputBaseFilename=rtstft_ctl-windows
 
[Files]
Source: "rtstft_ctl.vst3"; DestDir: "{app}\VST3"; Flags: ignoreversion


[Dirs]
Name: "{userdocs}\sound_ctl\rtstft_ctl"

