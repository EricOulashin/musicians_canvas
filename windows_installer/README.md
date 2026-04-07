# Windows Installer (WiX)

This directory contains the [WiX Toolset v4](https://wixtoolset.org/) source for the
Musician's Canvas Windows installer (`.msi`).

## What it builds

A Windows Installer package that:

- Installs to `C:\Program Files\Musicians_Canvas`
- Includes `musicians_canvas.exe`, `virtual_midi_keyboard.exe`, all required DLLs,
  Qt translation files (`.qm`), and the user manual in HTML and PDF for every
  supported language
- Creates a Start Menu program group **Musician's Canvas** with shortcuts to both
  executables, plus a **Docs** subfolder containing shortcuts to every PDF and HTML
  user manual
- Registers an entry in **Settings → Apps → Installed apps** (Add/Remove Programs)
  so the application can be uninstalled normally

## Building locally

You need the .NET SDK and the WiX command-line tool:

```cmd
dotnet tool install --global wix
```

Stage the files (a built `Windows` artifact directory works) and then run:

```cmd
wix build windows_installer\musicians_canvas.wxs ^
    -arch x64 ^
    -d StagingDir=path\to\staging ^
    -d Version=0.1.1.0 ^
    -o MusiciansCanvas_011_Windows.msi
```

`StagingDir` must contain the same layout as the Windows zip artifact:
`musicians_canvas.exe`, `virtual_midi_keyboard.exe`, runtime DLLs, `translations\`,
and `docs\` (with `html\` and PDF files).

## Building in CI

The GitHub Actions workflow (`.github/workflows/ci.yml`) builds the MSI automatically
on every Windows build. It uploads the MSI as a separate artifact alongside the
existing zip artifact (which remains the portable distribution).

## Why WiX (not Visual Studio Installer Projects)?

- WiX is open source and produces the same MSI format used by all standard Windows
  installers.
- It works headlessly from a CLI, so it builds cleanly in CI without needing the
  Visual Studio IDE installed.
- Visual Studio Installer Projects only work inside the Visual Studio IDE and are
  not buildable on a headless CI runner.
