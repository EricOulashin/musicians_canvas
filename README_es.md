# Musician's Canvas

Esta es una aplicación de grabación musical multipista para PCs de escritorio. Está escrita en C++ y utiliza Qt como
biblioteca de interfaz gráfica. Esta aplicación está diseñada para Linux, Windows y Mac OS. La facilidad de uso es una
consideración principal del diseño. Este proyecto comenzó como un experimento con Cursor (herramienta de codificación con IA) para ver qué
sería capaz de crear como aplicación de este tipo, con uso continuado de desarrollo con IA así
como desarrollo de software tradicional.

Hace años, creé un programa de grabación musical multipista en la universidad (con el mismo nombre); este es un intento
de crear algo mejor.

## Características

- **Arreglo multipista**: Agregar y eliminar múltiples pistas para componer canciones
- **Pistas con nombre**: Cada pista tiene un nombre editable que se usa como nombre de archivo para el audio grabado
- **Tipos de pista**: Configurar cada pista para grabación de audio (desde micrófono/entrada de línea) o grabación MIDI (desde teclado/controlador); haga clic en el icono de tipo de pista para cambiar rápidamente la fuente de entrada
- **Preparación de pista**: Marque "Arm" en una pista para seleccionarla como objetivo de grabación; solo una pista puede estar preparada a la vez. Una única pista sin grabar se prepara automáticamente por conveniencia
- **Cuenta regresiva de grabación**: Una cuenta regresiva visual de 3 segundos antes de que comience la grabación, dando al intérprete tiempo para prepararse
- **Grabación overdub**: Al grabar una nueva pista mientras las pistas existentes están habilitadas, las pistas existentes se mezclan y reproducen en tiempo real para que pueda escucharlas mientras graba. La reproducción y la captura están sincronizadas para mantener todas las pistas alineadas
- **Retroalimentación visual**: Visualización de forma de onda de audio para pistas de audio (con medidor de nivel en vivo durante la grabación), vista de piano roll MIDI para pistas MIDI
- **Sintetizador MIDI integrado**: Renderiza pistas MIDI a audio usando FluidSynth con un SoundFont configurable
- **Mezclar a un solo archivo de audio**: Exportar todas las pistas habilitadas a un solo archivo WAV o FLAC mezclado usando la biblioteca [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)
- **Guardar / Abrir proyecto**: Serializar y restaurar el proyecto completo (pistas, nombres, tipos, notas MIDI, referencias de archivos de audio) a/desde un archivo JSON, con detección de cambios no guardados al salir
- **Configuración específica del proyecto**: Sobrescribir los valores predeterminados globales de MIDI y audio por proyecto (frecuencia de muestreo, SoundFont, dispositivo MIDI)
- **Captura con PortAudio (compilación opcional)**: Cuando el proyecto se compila con PortAudio (`HAVE_PORTAUDIO`), la grabación puede usar una ruta de entrada nativa de PortAudio (similar en espíritu a Audacity) en lugar de Qt Multimedia. **Project → Project Settings → Audio** permite elegir **PortAudio** o **Qt Multimedia** y seleccionar un dispositivo de entrada PortAudio. Si PortAudio no está instalado, la compilación tiene éxito igualmente y la grabación usa solo Qt Multimedia.
- **Conversión de frecuencia de muestreo de alta calidad**: Graba a la frecuencia nativa del dispositivo de audio y convierte a la frecuencia del proyecto usando interpolación sinc con ventana (ventana Kaiser, ~96 dB de atenuación en la banda de rechazo), la misma familia de algoritmos usada por Audacity / libsoxr. Esto permite grabar a cualquier frecuencia de muestreo del proyecto (8000 Hz a 192000 Hz) independientemente de la frecuencia nativa del dispositivo, sin cambio de tono ni duración.
- **Manejo automático mono/estéreo**: Detecta dispositivos físicamente mono que se anuncian como estéreo (común con micrófonos de webcam USB en PipeWire) y convierte entre mono y estéreo según sea necesario (duplicación o promediado), siguiendo el enfoque de enrutamiento de canales de Audacity
- **Efectos de inserción en grabación (pistas de audio)**: Use el botón **Effects** (debajo de **Options** en cada pista de audio) para abrir el diálogo **Track effects**. Añada y configure **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** y **Amp & cabinet** (modelado de amplificador y pantalla), arrastre el asa **≡** para reordenar la cadena (arriba se aplica primero) y guarde con el proyecto. Los efectos se aplican a la toma cuando detiene la grabación; los parámetros están en unidades reales (ms, Hz) para un comportamiento coherente en mono o estéreo y tasas de muestreo habituales del proyecto (8 kHz–192 kHz) tras normalizar la captura. Consulte el [manual del usuario](docs/MusiciansCanvas_User_Manual_es.md). Los traductores pueden actualizar las cadenas por lotes con `scripts/effect_i18n.tsv` (generado por `scripts/build_effect_tsv.py`) y `scripts/fill_effect_i18n.py` tras `lupdate`.
- **Efectos de mezcla (bus principal)**: En **Project → Project Settings → Mix Effects** configura una cadena sobre **toda la mezcla** (mismos tipos y orden que en pistas). Se aplica al **reproducir todas las pistas** y al **exportar la mezcla** (WAV/FLAC). Se guarda en `project.json` como `mixEffectChain`.
- **Anticlipping en la salida de efectos**: El DSP integrado limita suavemente antes del PCM de 16 bits para reducir el recorte digital duro. **EffectWidget** ofrece `guardFloatSampleForInt16Pcm()` y `softLimitFloatSampleForInt16Pcm()` para nuevo procesado en tiempo real.
- **Monitorizar al grabar**: La casilla **Escuchar el audio mientras se graba** (a la derecha del reloj numérico) envía la entrada en directo a la **salida de audio del proyecto** durante la captura. En **audio** es el mismo sonido que se graba (además del overdub). En **MIDI**, con **renderizar MIDI a audio para reproducción** y SoundFont, oye el sintetizador. Se guarda en el **proyecto** (`monitorWhileRecording`). Desactívela para evitar retroalimentación.
- **Audio de baja latencia**: En Windows, la detección del controlador ASIO asegura audio de baja latencia; en Linux, se eleva la prioridad de planificación del proceso para menor jitter con PipeWire / PulseAudio / ALSA
- **Añadir pista de batería**: **Tools → Añadir pista de batería** (atajo **D**) añade una pista MIDI en el canal 10 General MIDI (batería) y escribe un `.mid` corto de dos compases (bombo, caja, hi-hat) en la carpeta del proyecto. Si **Metrónomo** tiene activada **Activar metrónomo durante la grabación**, se usa ese BPM; si no, se estima a partir del audio habilitado o 120 BPM por defecto. Vea el [manual de usuario](docs/MusiciansCanvas_User_Manual_es.md).
- **Virtual MIDI Keyboard**: Una aplicación complementaria para enviar notas MIDI a través de un teclado de piano por software, con sintetizador FluidSynth integrado, ganancia maestra ajustable, mapeo de teclado de computadora a piano, selección de instrumento/programa, control de chorus/efecto y desplazamiento de octava
- **Configuración**: Selección de dispositivo de entrada de audio, dispositivo MIDI y archivo SoundFont (valores predeterminados globales y sobrescrituras por proyecto)
- **Tema oscuro / claro**: Configurable a través de Settings → Configuration
- **Localización**: Ambas aplicaciones están localizadas en 18 idiomas: inglés, alemán, español, francés, japonés, portugués (Brasil), chino tradicional, chino simplificado, ruso, sueco, finlandés, danés, noruego, polaco, griego, irlandés, galés y pirata. El idioma se establece por defecto según la configuración del sistema operativo y puede cambiarse en **Settings → Configuration → Language**. Se utiliza el sistema estándar i18n de Qt (`QTranslator` / `.ts` / `.qm`), y la interfaz se actualiza inmediatamente cuando se cambia el idioma.
- **Manual de usuario**: Documentación en HTML y PDF generada a partir de fuentes Markdown, disponible en todos los idiomas soportados (ver [docs/](docs/))

## Capturas de pantalla

<p align="center">
	<a href="screenshots/i18n/es/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_1_MainWin.png" alt="Ventana principal" width="800"></a>
	<a href="screenshots/i18n/es/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_2_TrackConfig.png" alt="Configuración de pista" width="800"></a>
	<a href="screenshots/i18n/es/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_3_GeneralSettings.png" alt="Configuración general" width="800"></a>
	<a href="screenshots/i18n/es/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_4_MIDISettings.png" alt="Configuración MIDI predeterminada/general" width="800"></a>
	<a href="screenshots/i18n/es/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_5_AudioSettings.png" alt="Configuración de audio predeterminada/general" width="800"></a>
	<a href="screenshots/i18n/es/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_DisplaySettings.png" alt="Configuración de pantalla" width="800"></a>
	<a href="screenshots/i18n/es/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_LanguageSettings.png" alt="Configuración de idioma" width="800"></a>
	<a href="screenshots/i18n/es/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Configuración MIDI específica del proyecto" width="800"></a>
	<a href="screenshots/i18n/es/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/es/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Configuración de audio específica del proyecto" width="800"></a>
	<a href="screenshots/i18n/es/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/es/Track_Effects_Dialog.png" alt="Diálogo Efectos de pista" width="800"></a>
	<a href="screenshots/i18n/es/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/es/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/es/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/es/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/es/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/es/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/es/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/es/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Dependencias

### Compilador

Se requiere un compilador compatible con **C++17** (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (requerido en todas las plataformas)

La funcionalidad de mezcla y exportación FLAC depende de la biblioteca
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp).
Clónela como un directorio hermano de este repositorio antes de compilar:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

La compilación con CMake espera encontrarlo en `../audio_mixer_cpp` relativo a la raíz de este proyecto.

---

### Qt6 Multimedia (opcional pero muy recomendado)

`qt6-multimedia` es opcional. Sin él, la aplicación se compila y ejecuta, pero
**la grabación y reproducción de audio están deshabilitadas** — solo funcionarán la edición MIDI y la gestión de proyectos. Instálelo junto con las bibliotecas principales de Qt6 usando los comandos de plataforma a continuación.

---

### PortAudio (opcional)

Si la biblioteca y los encabezados de PortAudio están instalados, CMake habilita **`HAVE_PORTAUDIO`** y
lo enlaza en **musicians_canvas**. La grabación entonces usa por defecto la ruta PortAudio a menos que
el proyecto esté configurado para **Qt Multimedia** en **Project → Project Settings → Audio**. Si PortAudio
no se encuentra, la compilación continúa sin él (el wrapper se compila como stubs).

---

### SoundFont para síntesis MIDI

Se requiere un archivo SoundFont (`.sf2`) para que las pistas MIDI produzcan audio. Sin uno,
las pistas MIDI se renderizarán como silencio. En **Linux**, el sintetizador FluidSynth integrado
detectará automáticamente un SoundFont si uno está instalado en una ruta estándar del sistema
(vea los nombres de paquetes a continuación). En **macOS y Windows** no hay una ruta estándar del sistema,
por lo que debe configurar el SoundFont manualmente en
**Settings → Configuration → MIDI**.

---

### Linux — Ubuntu / Debian

```bash
sudo apt install build-essential cmake \
  qt6-base-dev qt6-multimedia-dev \
  qt6-l10n-tools \
  libfluidsynth-dev librtmidi-dev libflac-dev libsndfile1-dev \
  libportaudio2 portaudio19-dev \
  libpipewire-0.3-dev \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` instala `FluidR3_GM.sf2` en `/usr/share/sounds/sf2/` y se
> detecta automáticamente al iniciar. `timgm6mb-soundfont` es una alternativa más pequeña.
>
> `libpipewire-0.3-dev` es necesario en sistemas basados en PipeWire para que Virtual MIDI
> Keyboard pueda llamar a `pw_init()` antes de que FluidSynth cree sus recursos de audio.
> La compilación procede sin él; simplemente no se define el indicador `HAVE_PIPEWIRE`.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` instala `FluidR3_GM.sf2` en `/usr/share/soundfonts/` y se
> detecta automáticamente al iniciar.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` instala `FluidR3_GM.sf2` en `/usr/share/soundfonts/` y se
> detecta automáticamente al iniciar. `pipewire` normalmente ya está instalado en sistemas
> Arch modernos; los encabezados de desarrollo están incluidos en el paquete principal.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> PipeWire es un subsistema exclusivo de Linux y **no** es necesario en macOS. FluidSynth
> usará CoreAudio automáticamente a través del backend de Qt Multimedia.
>
> Descargue un SoundFont General MIDI (por ejemplo,
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) o
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> y configure su ruta en **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** requiere un **controlador de audio ASIO** para grabación y reproducción de baja latencia.
La aplicación no se iniciará si no se detecta ningún controlador ASIO.
**virtual_midi_keyboard** no requiere ASIO y usa directamente el backend WASAPI de Qt Multimedia.

**Instalación de un controlador ASIO para musicians_canvas (elija uno):**

| Controlador | Notas |
|--------------|-------|
| [ASIO4ALL](https://asio4all.org/) | Gratuito, funciona con la mayoría del hardware de audio integrado y USB |
| Controlador del fabricante | Mejor latencia para interfaces de audio dedicadas (por ejemplo, Focusrite, PreSonus, RME) |

**Cadena de herramientas — MSYS2 (recomendado para compilaciones MinGW):**

Descargue e instale MSYS2 desde <https://www.msys2.org>, luego agregue
`C:\msys64\mingw64\bin` a su `PATH` del sistema. Instale los paquetes requeridos:

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
pacman -S mingw-w64-x86_64-libsndfile
pacman -S mingw-w64-x86_64-portaudio
pacman -S mingw-w64-x86_64-soundfont-fluid
```

> `mingw-w64-x86_64-soundfont-fluid` instala `FluidR3_GM.sf2` en
> `C:\msys64\mingw64\share\soundfonts\` (ajuste si MSYS2 está instalado en otra ubicación).
> Debido a que Windows no tiene una ruta de búsqueda estándar de SoundFont, debe configurar esta ruta
> manualmente en **Settings → Configuration → MIDI** después del primer inicio.
>
> PipeWire es un subsistema exclusivo de Linux; no se necesita ningún paquete de PipeWire en Windows.

Páginas de referencia de paquetes:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Cadena de herramientas — Visual Studio 2022:**

Instale Qt 6 a través del [Qt Online Installer](https://www.qt.io/download) y obtenga
los binarios de FluidSynth, RtMidi y libFLAC (o compílelos desde el código fuente).
La compilación con CMake los localizará mediante `find_library` / `find_path` siempre que los
directorios apropiados estén en `CMAKE_PREFIX_PATH`.

Se debe descargar un SoundFont por separado (por ejemplo,
[GeneralUser GS](https://schristiancollins.com/generaluser.php) o
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) y su
ruta debe configurarse en **Settings → Configuration → MIDI** después del primer inicio.

> **Nota:** `advapi32` (registro de Windows) y `winmm` (Windows Multimedia) se
> enlazan automáticamente por CMake; no se requiere instalación adicional para estos.

---

## Compilación

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

**Windows (Visual Studio 2022):**

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Si las DLL requeridas no se encuentran en tiempo de ejecución, ejecute el script auxiliar desde el directorio de salida de compilación:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Generación de documentación

El manual de usuario se puede generar como HTML y PDF desde la fuente Markdown:

```bash
cd docs
./generate_docs.sh          # Generar HTML y PDF
./generate_docs.sh html     # Generar solo HTML
./generate_docs.sh pdf      # Generar solo PDF
```

**Prerrequisitos:**

- **Python (preferido):** `pip3 install markdown weasyprint`
- **Alternativa:** `pandoc` y `wkhtmltopdf` (mediante el gestor de paquetes del sistema)

El HTML generado se escribe en `docs/html/` y el PDF en `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Ejecución

```bash
./build/musicians_canvas
# o
./build/virtual_midi_keyboard   # aplicación complementaria Virtual MIDI Keyboard
```

---

## Uso

### musicians_canvas

1. **Establecer directorio del proyecto**: Ingrese o busque una carpeta donde se almacenarán las grabaciones y el archivo del proyecto
2. **Agregar pistas**: Haga clic en "+ Add Track"; nombre cada pista en el campo de texto junto a "Options"
3. **Configurar tipo de pista**: Haga clic en "Options" en una pista (o haga clic en el icono de tipo de pista entre "Options" y el campo de nombre) para elegir Audio o MIDI y establecer la fuente de entrada
4. **Eliminar una pista**: Haga clic en el botón "×" en el lado derecho de la fila de la pista
5. **Configuración global**: Use **Settings → Configuration** para establecer valores predeterminados globales:
   - Pestaña *General*: Tema (oscuro/claro)
   - Pestaña *MIDI*: Dispositivo de salida MIDI predeterminado (sintetizador FluidSynth integrado o un dispositivo MIDI externo) y SoundFont predeterminado (`.sf2`) para síntesis
   - Pestaña *Audio*: Dispositivo de entrada/salida de audio predeterminado para grabación y reproducción
6. **Configuración del proyecto**: Use **Project → Project Settings** (Ctrl+P) para sobrescribir la configuración MIDI y de audio solo para el proyecto actual (por ejemplo, una frecuencia de muestreo o SoundFont diferente por canción)
7. **Grabar**: Marque "Arm" en la pista objetivo, luego haga clic en el botón de grabación (círculo rojo). Solo una pista puede estar preparada a la vez
8. **Reproducir**: Haga clic en el botón de reproducción para mezclar y reproducir todas las pistas habilitadas
9. **Mezclar a archivo**: Use **Tools → Mix tracks to file** (Ctrl+M) para exportar a WAV o FLAC
10. **Guardar / Abrir**: Use **File → Save Project** (Ctrl+S) y **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Abrir configuración**: Use el botón o menú **Configuration** (Ctrl+,) para abrir el diálogo de configuración
2. **Pestaña MIDI**:
   - Seleccione un dispositivo de salida MIDI (un sintetizador externo de hardware/software) o déjelo en blanco para usar el sintetizador FluidSynth integrado
   - Seleccione un dispositivo de entrada MIDI para reenviar notas MIDI entrantes a la visualización del teclado
   - Ajuste el **Synthesizer Volume (Master Gain)** para controlar el nivel de salida del sintetizador integrado (10 %–200 %)
3. **Pestaña Audio**: Seleccione el dispositivo de salida de audio utilizado por el sintetizador integrado
4. **SoundFont**: Seleccione un archivo SoundFont `.sf2` para el sintetizador integrado (detectado automáticamente en Linux si hay un SoundFont del sistema instalado)
5. **Tocar notas**: Haga clic en las teclas del teclado de piano en pantalla, use un controlador MIDI conectado o use el teclado de la computadora:
   - Octava inferior: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = sostenidos/bemoles
   - Octava superior: Q/W/E/R/T/Y/U/I/O/P = C hasta E, 2/3/5/6/7/9/0 = sostenidos/bemoles
6. **Controles de la barra de herramientas**: Ajustar volumen MIDI (0–127), octava (-3 a +5), nivel de chorus/efecto y seleccionar instrumentos

---

## Atajos de teclado

**musicians_canvas:**

| Atajo | Acción |
|-------|--------|
| Ctrl+S | Guardar proyecto |
| Ctrl+O | Abrir proyecto |
| Ctrl+M | Mezclar pistas a archivo |
| Ctrl+P | Configuración del proyecto |
| Ctrl+, | Ajustes / Configuración |
| Ctrl+Q / Alt+F4 | Salir |

**virtual_midi_keyboard:**

| Atajo | Acción |
|-------|--------|
| Ctrl+, | Diálogo de configuración |
| Ctrl+U | Ayuda / Información de uso |
| Ctrl+Q | Cerrar |

---

## Licencia

Este proyecto se proporciona tal cual para uso educativo y personal.
