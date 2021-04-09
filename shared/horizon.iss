#define Output "."

[Setup]
AppId={{ecb0429a-67bc-47fe-9f47-aaec1f405054}
AppName=Horizon
AppVersion={#AppVersion}
AppVerName=Horizon {#AppVersion}
AppPublisher=Tom Butcher
AppPublisherURL=https://github.com/tom-23/Horizon
AppSupportURL=https://github.com/tom-23/Horizon
AppUpdatesURL=https://github.com/tom-23/Horizon
DefaultDirName={pf}\Horizon
DefaultGroupName=Horizon
AllowNoIcons=yes
LicenseFile={#Source}\LICENSE
OutputDir={#Output}
OutputBaseFilename=horizon-{#AppVersion}-setup
Compression=lzma
SolidCompression=yes
SetupIconFile=installicon.ico
WizardImageFile=logo.bmp
WizardSmallImageFile=logo-small.bmp

[Languages]
Name: en; MessagesFile: "compiler:Default.isl"

[CustomMessages]
en.ProgramFiles=Program Files

[Types]
Name: "full"; Description: "{code:GetFullInstallation}"

[Components]
Name: "program"; Description: "{cm:ProgramFiles}"; Types: full ; Flags: fixed

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; Flags: unchecked

[Files]
Source: "{#Root}\horizon.exe"; DestDir: "{app}"; Components: program; Flags: ignoreversion
Source: "{#Root}\horizonengine.exe"; DestDir: "{app}"; Components: program; Flags: ignoreversion
Source: "{#Root}\LICENSE"; DestDir: "{app}"; Components: program; Flags: ignoreversion
Source: "{#Root}\README.md"; DestDir: "{app}"; Components: program; Flags: ignoreversion

; Qt and toolchain
Source: "{#Root}\bearer\*.dll"; DestDir: "{app}\bearer"; Components: program; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#Root}\imageformats\*.dll"; DestDir: "{app}\imageformats"; Components: program; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#Root}\platforms\*.dll"; DestDir: "{app}\platforms"; Components: program; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#Root}\styles\*.dll"; DestDir: "{app}\styles"; Components: program; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#Root}\*.dll"; DestDir: "{app}"; Components: program; Flags: ignoreversion

[Icons]
Name: "{group}\Horizon"; Filename: "{app}\horizon.exe"
Name: "{commondesktop}\Horizon"; Filename: "{app}\horizon.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\horizon.exe"; Description: "{cm:LaunchProgram,Horizon}"; Flags: nowait postinstall skipifsilent

[Code]
function GetFullInstallation(Param: string): string;
begin
	Result := SetupMessage(msgFullInstallation);
end;