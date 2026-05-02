# Windows Installer (Inno Setup)

This directory contains the [Inno Setup](https://jrsoftware.org/isinfo.php) source for
the Musician's Canvas Windows installer (`.exe`).

## What it builds

A Windows setup executable that:

- Installs to `C:\Program Files\Musicians_Canvas`
- Includes `musicians_canvas.exe`, `virtual_midi_keyboard.exe`, all required DLLs,
  Qt translation files (`.qm`), and the user manual in HTML and PDF for every
  supported language
- Creates a Start Menu program group **Musician's Canvas** with shortcuts to both
  executables, an uninstall shortcut, and a **Docs** subfolder containing shortcuts
  to every PDF and HTML user manual
- Optionally creates a desktop shortcut (off by default; user can opt in during setup)
- Registers an entry in **Settings → Apps → Installed apps** (Add/Remove Programs)
  so the application can be uninstalled normally

## Building locally

Install [Inno Setup 6](https://jrsoftware.org/isdl.php) (or via Chocolatey:
`choco install innosetup -y`), then stage the files (a built `Windows` artifact
directory works) and run:

```cmd
iscc /Qp ^
    "/DStagingDir=path\to\staging" ^
    "/DAppVersion=0.1.2" ^
    "/DOutputBaseName=MusiciansCanvas_011_Windows_Setup" ^
    windows_installer\musicians_canvas.iss
```

`StagingDir` must contain the same layout as the Windows zip artifact:
`musicians_canvas.exe`, `virtual_midi_keyboard.exe`, runtime DLLs, `translations\`,
and `docs\` (with `html\` and PDF files).

## Building in CI

The GitHub Actions workflow (`.github/workflows/ci.yml`) builds the installer
automatically on every Windows build. It installs Inno Setup via Chocolatey and
uploads the resulting `.exe` as a separate artifact alongside the existing zip
artifact (which remains the portable distribution).

## Why Inno Setup?

- Free, simple licensing — no EULA prompts or version-pinning hurdles
- Concise script syntax (`.iss`) compared to MSI/WiX XML
- Produces a single self-contained `.exe` setup wizard, which is the most familiar
  installer experience for end users on Windows
- First-class support in CI: pre-installed compiler (`iscc.exe`) is available via
  Chocolatey on standard `windows-latest` GitHub runners
- All required features (Program Files install dir, Start Menu group with subfolder,
  uninstaller entry, recursive file harvesting) are built-in and trivially expressed
