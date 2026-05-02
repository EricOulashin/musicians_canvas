; Inno Setup script for the Musician's Canvas Windows installer.
; See windows_installer/README.md for build instructions and a feature overview.
;
; Build (locally or in CI):
;   iscc /Qp /DStagingDir=path\to\staging /DAppVersion=0.1.2 musicians_canvas.iss
;
; The /DStagingDir variable must point to a directory containing the same layout
; as the Windows zip artifact (musicians_canvas.exe, virtual_midi_keyboard.exe,
; all DLLs, translations\, docs\html\, docs\*.pdf).

#ifndef StagingDir
  #error StagingDir must be defined on the command line: iscc /DStagingDir=path
#endif
#ifndef AppVersion
  #define AppVersion "0.1.2"
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
; Start Menu shortcut location: per-user Roaming folder
;   %APPDATA%\Microsoft\Windows\Start Menu\Programs\Musician's Canvas
; The {userprograms} constant resolves at install time to the current user's
; Start Menu Programs folder.
#define StartMenuDir   "{userprograms}\Musician's Canvas"
#define DocsMenuDir    "{userprograms}\Musician's Canvas\Docs"

; User settings (NOT shipped in the installer payload) -------------------------
; Both applications store their configuration as INI files next to the
; executables in the install directory:
;   - musicians_canvas.exe  ->  config.ini           (see src/appsettings.cpp)
;   - virtual_midi_keyboard.exe  ->  virtual_keyboard_config.ini  (see src/virtual_midi_keyboard/vk_settings.cpp)
; Upgrades use the same stable AppId below, so the install folder is updated in
; place. Inno Setup only overwrites files that are part of this [Files] payload;
; runtime-created INIs are left untouched. The [Files] Excludes list below is a
; safeguard so a future staging tree cannot accidentally ship and overwrite
; those names.

[Setup]
; Stable AppId: required for upgrades.  When this GUID matches an existing
; install (Uninstall registry key), Setup runs as an update: same {app} path by
; default, destination wizard page suppressed (DisableDirPage=auto), files from
; [Files] replaced (see ignoreversion below).  Do not change AppId or users
; will get a second, parallel install.
AppId={{B7E5F2A8-8C4D-4F1E-9B2A-3D4E5F6A7B8C}
AppName={#AppName}
AppVersion={#AppVersion}
AppVerName={#AppName} {#AppVersion}
AppPublisher={#AppPublisher}
AppPublisherURL={#AppURL}
AppSupportURL={#AppURL}
AppUpdatesURL={#AppURL}
; Reuse the prior install directory from the registry when upgrading (Inno default
; is yes; kept explicit so a future edit does not break in-place updates).
UsePreviousAppDir=yes
; Hide "Select Destination Location" when updating an existing install (Inno default
; is auto; explicit for the same reason as UsePreviousAppDir).
DisableDirPage=auto
DefaultDirName={commonpf64}\{#InstallSubdir}
DefaultGroupName={#AppName}
DisableProgramGroupPage=yes
; Allow an admin-mode install to write Start Menu shortcuts into the per-user
; Roaming folder ({userprograms}) without showing the "user-area" warning.
UsedUserAreasWarning=no
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
; Inno default is yes: use Restart Manager so locked EXEs/DLLs can be closed before
; replacing files during an upgrade (see [Files] ignoreversion).
CloseApplications=yes
OutputBaseFilename={#OutputBaseName}
; Embed version metadata in the setup .exe (visible in File Properties).
VersionInfoVersion={#AppVersion}
VersionInfoCompany={#AppPublisher}
VersionInfoDescription={#AppName} Installer
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
; Never install packaged copies of user-specific config files (see comment block
; above); existing files from a prior install must be preserved on upgrade.
Source: "{#StagingDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Excludes: "config.ini;virtual_keyboard_config.ini"

[Icons]
; --- Main program shortcuts in the per-user Start Menu group
;     ({userprograms}\Musician's Canvas) ---
Name: "{#StartMenuDir}\Musician's Canvas"; Filename: "{app}\{#AppExeName}"; WorkingDir: "{app}"; Comment: "Multi-track music recording"
Name: "{#StartMenuDir}\Virtual MIDI Keyboard"; Filename: "{app}\{#VkExeName}"; WorkingDir: "{app}"; Comment: "On-screen virtual MIDI keyboard"

; Optional desktop shortcuts for both applications
Name: "{commondesktop}\Musician's Canvas"; Filename: "{app}\{#AppExeName}"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{commondesktop}\Virtual MIDI Keyboard"; Filename: "{app}\{#VkExeName}"; WorkingDir: "{app}"; Tasks: desktopicon

; Uninstall shortcut in the per-user Start Menu group
Name: "{#StartMenuDir}\Uninstall {#AppName}"; Filename: "{uninstallexe}"

; --- Docs subfolder under the per-user Start Menu group ---
; HTML user manuals
Name: "{#DocsMenuDir}\User Manual - English (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual.html"
Name: "{#DocsMenuDir}\User Manual - Deutsch (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_de.html"
Name: "{#DocsMenuDir}\User Manual - Espanol (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_es.html"
Name: "{#DocsMenuDir}\User Manual - Francais (HTML)";             Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_fr.html"
Name: "{#DocsMenuDir}\User Manual - Japanese (HTML)";             Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_ja.html"
Name: "{#DocsMenuDir}\User Manual - Portugues BR (HTML)";         Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_pt-BR.html"
Name: "{#DocsMenuDir}\User Manual - Chinese Traditional (HTML)";  Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_zh-TW.html"
Name: "{#DocsMenuDir}\User Manual - Chinese Simplified (HTML)";   Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_zh-CN.html"
Name: "{#DocsMenuDir}\User Manual - Russian (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_ru.html"
Name: "{#DocsMenuDir}\User Manual - Svenska (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_sv.html"
Name: "{#DocsMenuDir}\User Manual - Suomi (HTML)";                Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_fi.html"
Name: "{#DocsMenuDir}\User Manual - Dansk (HTML)";                Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_da.html"
Name: "{#DocsMenuDir}\User Manual - Norsk (HTML)";                Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_nb.html"
Name: "{#DocsMenuDir}\User Manual - Polski (HTML)";               Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_pl.html"
Name: "{#DocsMenuDir}\User Manual - Greek (HTML)";                Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_el.html"
Name: "{#DocsMenuDir}\User Manual - Gaeilge (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_ga.html"
Name: "{#DocsMenuDir}\User Manual - Cymraeg (HTML)";              Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_cy.html"
Name: "{#DocsMenuDir}\User Manual - Pirate (HTML)";               Filename: "{app}\docs\html\MusiciansCanvas_User_Manual_pirate.html"

; PDF user manuals
Name: "{#DocsMenuDir}\User Manual - English (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual.pdf"
Name: "{#DocsMenuDir}\User Manual - Deutsch (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_de.pdf"
Name: "{#DocsMenuDir}\User Manual - Espanol (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_es.pdf"
Name: "{#DocsMenuDir}\User Manual - Francais (PDF)";              Filename: "{app}\docs\MusiciansCanvas_User_Manual_fr.pdf"
Name: "{#DocsMenuDir}\User Manual - Japanese (PDF)";              Filename: "{app}\docs\MusiciansCanvas_User_Manual_ja.pdf"
Name: "{#DocsMenuDir}\User Manual - Portugues BR (PDF)";          Filename: "{app}\docs\MusiciansCanvas_User_Manual_pt-BR.pdf"
Name: "{#DocsMenuDir}\User Manual - Chinese Traditional (PDF)";   Filename: "{app}\docs\MusiciansCanvas_User_Manual_zh-TW.pdf"
Name: "{#DocsMenuDir}\User Manual - Chinese Simplified (PDF)";    Filename: "{app}\docs\MusiciansCanvas_User_Manual_zh-CN.pdf"
Name: "{#DocsMenuDir}\User Manual - Russian (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_ru.pdf"
Name: "{#DocsMenuDir}\User Manual - Svenska (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_sv.pdf"
Name: "{#DocsMenuDir}\User Manual - Suomi (PDF)";                 Filename: "{app}\docs\MusiciansCanvas_User_Manual_fi.pdf"
Name: "{#DocsMenuDir}\User Manual - Dansk (PDF)";                 Filename: "{app}\docs\MusiciansCanvas_User_Manual_da.pdf"
Name: "{#DocsMenuDir}\User Manual - Norsk (PDF)";                 Filename: "{app}\docs\MusiciansCanvas_User_Manual_nb.pdf"
Name: "{#DocsMenuDir}\User Manual - Polski (PDF)";                Filename: "{app}\docs\MusiciansCanvas_User_Manual_pl.pdf"
Name: "{#DocsMenuDir}\User Manual - Greek (PDF)";                 Filename: "{app}\docs\MusiciansCanvas_User_Manual_el.pdf"
Name: "{#DocsMenuDir}\User Manual - Gaeilge (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_ga.pdf"
Name: "{#DocsMenuDir}\User Manual - Cymraeg (PDF)";               Filename: "{app}\docs\MusiciansCanvas_User_Manual_cy.pdf"
Name: "{#DocsMenuDir}\User Manual - Pirate (PDF)";                Filename: "{app}\docs\MusiciansCanvas_User_Manual_pirate.pdf"

[UninstallDelete]
; Remove the entire "Musician's Canvas" Start Menu folder (including the Docs
; subfolder) on uninstall, even if any stray files remain in it.  Inno Setup
; tracks and removes the individual shortcuts it created automatically; this
; entry is a belt-and-braces cleanup of the parent folders.
Type: filesandordirs; Name: "{#DocsMenuDir}"
Type: filesandordirs; Name: "{#StartMenuDir}"

[Run]
Filename: "{app}\{#AppExeName}"; Description: "Launch {#AppName}"; Flags: nowait postinstall skipifsilent
