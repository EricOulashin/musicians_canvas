# Localized screenshots

## OCR / “translate the image”

**No.** There’s no practical OCR → translate → repaint pipeline here (no OCR stack in the environment, and redoing Qt UI text in a PNG wouldn’t match real fonts/layout). **Real localized screenshots** mean running the app in that locale and exporting new PNGs.

## What was implemented instead

1. **`MUSICIANS_CANVAS_LANG`**  
   If this environment variable is set at startup, it **overrides** the saved language for loading Qt/app translations (so you can capture screenshots without changing settings):
   - `src/musicians_canvas_main.cpp`
   - `src/virtual_midi_keyboard/virtual_midi_keyboard_main.cpp` (same variable for consistency)

2. **`screenshots/i18n/<locale>/`**  
   For each supported locale (`de`, `es`, `fr`, `ja`, `pt_BR`, `zh_CN`, `zh_TW`, `nb`, `pl`, `ru`, `sv`, `fi`, `da`, `el`, `ga`, `cy`, `pirate`), there is a folder with **symlinks** to the current English PNGs (same filenames). Replacing a symlink with a real file gives a true localized asset without changing markdown again.

3. **Localized docs**  
   - All `README_*.md` (except the root English `README.md`) now use `screenshots/i18n/<locale>/...`.  
   - All `docs/MusiciansCanvas_User_Manual_*.md` (not the English `MusiciansCanvas_User_Manual.md`) now use `../screenshots/i18n/<locale>/...`.

4. **English `README.md`**  
   Short note under **Screenshots** explaining the `i18n` layout, symlinks, and `MUSICIANS_CANVAS_LANG`.

## Getting real translated screenshots

Example:

```bash
MUSICIANS_CANVAS_LANG=de ./musicians_canvas
```

Then save PNGs over the files in `screenshots/i18n/de/` (keeping the same names as in `screenshots/`).

**Note:** `screenshots/i18n/` should be tracked in git; symlinks behave best on Unix. On Windows, ensure Git symlink support if you rely on them.

There is **no** `README_pirate.md`; the pirate manual **does** use `screenshots/i18n/pirate/`.
