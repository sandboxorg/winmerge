; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=WinMerge
AppVerName=WinMerge 2.0
AppPublisher=Thingamahoochie Software
AppPublisherURL=http://winmerge.sourceforge.net/
AppVersion=2.1.0.0
AppSupportURL=http://winmerge.sourceforge.net/
AppUpdatesURL=http://winmerge.sourceforge.net/
DefaultDirName={pf}\WinMerge
DefaultGroupName=WinMerge
DisableStartupPrompt=yes
AllowNoIcons=yes
LicenseFile=..\WinMerge\COPYING
InfoBeforeFile=Info.txt
OutputBaseFilename=WinMergeSetup
PrivilegesRequired=admin
UninstallDisplayIcon={app}\WinMerge.exe
WizardImageBackColor=$c0c0c0
WizardImageFile=WinMergeLarge.bmp
WizardSmallImageFile=WinMergeIcon.bmp

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"
Name: "quicklaunchicon"; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Components]
Name: "main"; Description: "WinMerge main files"; Types: full compact custom; Flags: fixed
Name: "docs"; Description: "User's guide"; Types: full
Name: "filters"; Description: "Filter files"; Types: full
Name: "brazilian"; Description: "Portuguese (Brazilian) menus and dialogs"; Types: full
Name: "chinesesimplifiedlanguage"; Description: "Chinese (simplified) menus and dialogs"; Types: full
Name: "chinesetraditionallanguage"; Description: "Chinese (traditional) menus and dialogs"; Types: full
Name: "czechlanguage"; Description: "Czech menus and dialogs"; Types: full
Name: "danishlanguage"; Description: "Danish menus and dialogs"; Types: full
Name: "dutchlanguage"; Description: "Dutch menus and dialogs"; Types: full
Name: "frenchlanguage"; Description: "French menus and dialogs"; Types: full
Name: "germanlanguage"; Description: "German menus and dialogs"; Types: full
Name: "italianlanguage"; Description: "Italian menus and dialogs"; Types: full
Name: "koreanlanguage"; Description: "Korean menus and dialogs"; Types: full
Name: "slovaklanguage"; Description: "Slovak menus and dialogs"; Types: full
Name: "spanishlanguage"; Description: "Spanish menus and dialogs"; Types: full

[Files]
Source: "..\Build\MergeRelease\WinMerge.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "..\Docs\*.*"; DestDir: "{app}\Docs"; Flags: ignoreversion; Components: docs
Source: "..\Filters\*.*"; DestDir: "{app}\Filters"; Flags: ignoreversion; Components: filters
Source: "..\Languages\DLL\MergeBrazilian.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: brazilian
Source: "..\Languages\DLL\MergeChineseSimplified.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: chinesesimplifiedlanguage
Source: "..\Languages\DLL\MergeChineseTraditional.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: chinesetraditionallanguage
Source: "..\Languages\DLL\MergeCzech.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: czechlanguage
Source: "..\Languages\DLL\MergeDanish.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: danishlanguage
Source: "..\Languages\DLL\MergeDutch.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: dutchlanguage
Source: "..\Languages\DLL\MergeFrench.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: frenchlanguage
Source: "..\Languages\DLL\MergeGerman.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: germanlanguage
Source: "..\Languages\DLL\MergeItalian.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: italianlanguage
Source: "..\Languages\DLL\MergeKorean.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: koreanlanguage
Source: "..\Languages\DLL\MergeSlovak.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: slovaklanguage
Source: "..\Languages\DLL\MergeSpanish.lang"; DestDir: "{app}"; Flags: ignoreversion; Components: spanishlanguage
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

; begin VC system files
Source: "VCRedist\mfc42.dll";    DestDir: "{sys}"; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall regserver
Source: "VCRedist\mfc42u.dll";   DestDir: "{sys}"; MinVersion: 0, 4; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall regserver
Source: "VCRedist\msvcrt.dll";   DestDir: "{sys}"; CopyMode: alwaysskipifsameorolder; Flags: restartreplace uninsneveruninstall
; end VC system files

[INI]
Filename: "{app}\WinMerge.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://winmerge.sourceforge.net/"

[Icons]
Name: "{group}\WinMerge"; Filename: "{app}\WinMerge.exe"
Name: "{group}\WinMerge on the Web"; Filename: "{app}\WinMerge.url"
Name: "{group}\Uninstall WinMerge"; Filename: "{uninstallexe}"
Name: "{userdesktop}\WinMerge"; Filename: "{app}\WinMerge.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\WinMerge"; Filename: "{app}\WinMerge.exe"; Tasks: quicklaunchicon

[Registry]
Root: HKCU; Subkey: "Software\Thingamahoochie"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"
; Set the default font to Courier New size 12
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: string; ValueName: "FaceName"; ValueData: "Courier New"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Specified"; ValueData: "1"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Height"; ValueData: "$fffffff0"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Width"; ValueData: "0"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Escapement"; ValueData: "0"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Orientation"; ValueData: "0"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Weight"; ValueData: "$190"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Italic"; ValueData: "0"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Underline"; ValueData: "0"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "StrikeOut"; ValueData: "0"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "CharSet"; ValueData: "0"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "OutPrecision"; ValueData: "3"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "ClipPrecision"; ValueData: "2"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "Quality"; ValueData: "1"
Root: HKCU; Subkey: "Software\Thingamahoochie\WinMerge\Font"; ValueType: dword; ValueName: "PitchAndFamily"; ValueData: "$31"

[Run]
Filename: "{app}\WinMerge.exe"; Description: "Launch WinMerge"; Flags: nowait postinstall skipifsilent unchecked

[InstallDelete]
; Diff.txt is a file left over from previous versions of WinMerge (before version 2.0), we just delete it to be nice.
Type: files; Name: "{app}\Diff.txt"

; A few users might have some intermediate Chinese translations on their machines (from version 2.0.0.2), we just delete those to be nice.
Type: files; Name: "{app}\MergeChineseSimplifiedGB2312.lang"
Type: files; Name: "{app}\MergeChineseTraditionalBIG5.lang"

[UninstallDelete]
Type: files; Name: "{app}\WinMerge.url"

