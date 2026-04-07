; Inno Setup script for the Musician's Canvas Windows installer.
; See windows_installer/README.md for build instructions and a feature overview.
;
; Build (locally or in CI):
;   iscc /Qp /DStagingDir=path\to\staging /DAppVersion=0.1.1 musicians_canvas.iss
;
; The /DStagingDir variable must point to a directory containing the same layout
; as the Windows zip artifact (musicians_canvas.exe, virtual_midi_keyboard.exe,
; all DLLs, translations\, docs\html\, docs\*.pdf).

#ifndef StagingDir
  #error StagingDir must be defined on the command line: iscc /DStagingDir=path
#endif
#ifndef AppVersion
  #define AppVersion "0.1.1"
#endif
#ifndef OutputBaseName
  #define OutputBaseName "MusiciansCanvas_Windows_Setup"
#endif

#define AppName        "Musician's Canvas"
#define AppPublisher   "Eric Oulashin"
#define AppURL         "https://github.com/EricOulashin/musicians_canvas"
#define AppExeName     "musicians_canvas.exe"
#define VkExeName      "virtual_midi_keyboard.exe"
#define InstallSubdir  "Musicians_Canvas"

[Setup]
; Stable AppId so upgrades replace the previous install
AppId={{B7E5F2A8-8C4D-4F1E-9B2A-3D4E5F6A7B8C}
AppName={#AppName}
AppVersion={#AppVersion}
AppVerName={#AppName} {#AppVersion}
AppPublisher={#AppPublisher}
AppPublisherURL={#AppURL}
AppSupportURL={#AppURL}
AppUpdatesURL={#AppURL}
DefaultDirName={commonpf64}\{#InstallSubdir}
DefaultGroupName={#AppName}
DisableProgramGroupPage=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
OutputBaseFilename={#OutputBaseName}
; Maximum compression: LZMA2 with the ultra64 preset (the highest preset Inno
; Setup offers).  Combined with solid compression, a large dictionary, the
; binary-tree match finder, the maximum number of fast bytes, and a separate
; compression process, this minimises the resulting installer size as much as
; Inno Setup allows.  Compression takes longer at build time but the install
; experience is unchanged.
Compression=lzma2/ultra64
SolidCompression=yes
LZMAUseSeparateProcess=yes
LZMADictionarySize=1048576
LZMANumFastBytes=273
LZMAMatchFinder=BT
LZMAAlgorithm=1
LZMANumBlockThreads=2
WizardStyle=modern
UninstallDisplayName={#AppName} {#AppVersion}
UninstallDisplayIcon={app}\{#AppExeName}
SetupIconFile={#StagingDir}\..\musicians_canvas\src\icons\MusiciansCanvas.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Create &desktop shortcuts (Musician's Canvas and Virtual MIDI Keyboard)"; GroupDescription: "Additional shortcuts:"; Flags: unchecked

[Files]
; Recursively include EVERYTHING from the staging directory.  This picks up
; both executables, all DLLs, translations\*.qm, docs\html\**, docs\*.pdf, etc.
Source: "{#StagingDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
; --- Main program shortcuts in the Start Menu group ---
Name: "{group}\Musician's Canvas"; Filename: "{app}\{#AppExeName}"; WorkingDir: "{app}"; Comment: "Multi-track music recording"
Name: "{group}\Virtual MIDI Keyboard"; Filename: "{app}\{#VkExeName}"; WorkingDir: "{app}"; Comment: "On-screen virtual MIDI keyboard"

; Optional desktop shortcuts for both applications
Name: "{commondesktop}\Musician's Canvas"; Filename: "{app}\{#AppExeName}"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{commondesktop}\Virtual MIDI Keyboard"; Filename: "{app}\{#VkExeName}"; WorkingDir: "{app}"; Tasks: desktopicon

; Uninstall shortcut in the Start Menu group
Name: "{group}\Uninstall {#AppName}"; Filename: "{uninstallexe}"

; --- Docs subfolder under the Start Menu group ---
; HTML user manuals
Name: "{group}\Docs\User Manual - English (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual.html"
Name: "{group}\Docs\User Manual - Deutsch (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_de.html"
Name: "{group}\Docs\User Manual - Espanol (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_es.html"
Name: "{group}\Docs\User Manual - Francais (HTML)";             Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_fr.html"
Name: "{group}\Docs\User Manual - Japanese (HTML)";             Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_ja.html"
Name: "{group}\Docs\User Manual - Portugues BR (HTML)";         Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_pt-BR.html"
Name: "{group}\Docs\User Manual - Chinese Traditional (HTML)";  Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_zh-TW.html"
Name: "{group}\Docs\User Manual - Chinese Simplified (HTML)";   Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_zh-CN.html"
Name: "{group}\Docs\User Manual - Russian (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_ru.html"
Name: "{group}\Docs\User Manual - Svenska (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_sv.html"
Name: "{group}\Docs\User Manual - Suomi (HTML)";                Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_fi.html"
Name: "{group}\Docs\User Manual - Dansk (HTML)";                Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_da.html"
Name: "{group}\Docs\User Manual - Norsk (HTML)";                Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_nb.html"
Name: "{group}\Docs\User Manual - Polski (HTML)";               Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_pl.html"
Name: "{group}\Docs\User Manual - Greek (HTML)";                Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_el.html"
Name: "{group}\Docs\User Manual - Gaeilge (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_ga.html"
Name: "{group}\Docs\User Manual - Cymraeg (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_cy.html"
Name: "{group}\Docs\User Manual - Pirate (HTML)";               Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_pirate.html"

; PDF user manuals
Name: "{group}\Docs\User Manual - English (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual.pdf"
Name: "{group}\Docs\User Manual - Deutsch (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_de.pdf"
Name: "{group}\Docs\User Manual - Espanol (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_es.pdf"
Name: "{group}\Docs\User Manual - Francais (PDF)";              Filename: "{app}\docs\MusiciansCanvas_User_Manual_fr.pdf"
Name: "{group}\Docs\User Manual - Japanese (PDF)";              Filename: "{app}\docs\MusiciansCanvas_User_Manual_ja.pdf"
Name: "{group}\Docs\User Manual - Portugues BR (PDF)";          Filename: "{app}\docs\MusiciansCanvas_User_Manual_pt-BR.pdf"
Name: "{group}\Docs\User Manual - Chinese Traditional (PDF)";   Filename: "{app}\docs\MusiciansCanvas_User_Manual_zh-TW.pdf"
Name: "{group}\Docs\User Manual - Chinese Simplified (PDF)";    Filename: "{app}\docs\MusiciansCanvas_User_Manual_zh-CN.pdf"
Name: "{group}\Docs\User Manual - Russian (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_ru.pdf"
Name: "{group}\Docs\User Manual - Svenska (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_sv.pdf"
Name: "{group}\Docs\User Manual - Suomi (PDF)";                 Filename: "{app}\docs\MusiciansCanvas_User_Manual_fi.pdf"
Name: "{group}\Docs\User Manual - Dansk (PDF)";                 Filename: "{app}\docs\MusiciansCanvas_User_Manual_da.pdf"
Name: "{group}\Docs\User Manual - Norsk (PDF)";                 Filename: "{app}\docs\MusiciansCanvas_User_Manual_nb.pdf"
Name: "{group}\Docs\User Manual - Polski (PDF)";                Filename: "{app}\docs\MusiciansCanvas_User_Manual_pl.pdf"
Name: "{group}\Docs\User Manual - Greek (PDF)";                 Filename: "{app}\docs\MusiciansCanvas_User_Manual_el.pdf"
Name: "{group}\Docs\User Manual - Gaeilge (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_ga.pdf"
Name: "{group}\Docs\User Manual - Cymraeg (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_cy.pdf"
Name: "{group}\Docs\User Manual - Pirate (PDF)";                Filename: "{app}\docs\MusiciansCanvas_User_Manual_pirate.pdf"

[Run]
Filename: "{app}\{#AppExeName}"; Description: "Launch {#AppName}"; Flags: nowait postinstall skipifsilent
