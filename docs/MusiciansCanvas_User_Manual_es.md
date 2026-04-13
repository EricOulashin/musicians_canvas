---
title: "Manual de Usuario de Musician's Canvas"
subtitle: "Aplicación de Grabación Musical Multipista"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "es"
---

# Manual de Usuario de Musician's Canvas

## Introducción

Musician's Canvas es una aplicación de grabación musical multipista para computadoras de escritorio. Soporta grabación de audio desde micrófonos y dispositivos de entrada de línea, grabación MIDI desde teclados y controladores, y mezcla de todas las pistas en un único archivo WAV o FLAC. Una aplicación complementaria, Virtual MIDI Keyboard, proporciona un teclado de piano por software para enviar notas MIDI.

Musician's Canvas está diseñado para ser fácil de usar y al mismo tiempo ofrecer funciones comúnmente encontradas en estaciones de trabajo de audio digital (DAWs):

- Grabación multipista de audio y MIDI
- Grabación en overdub con reproducción sincronizada de pistas existentes
- Sintetizador MIDI FluidSynth integrado con soporte de SoundFont
- Conversión de frecuencia de muestreo de alta calidad para grabar a cualquier frecuencia de muestreo del proyecto
- Detección automática de dispositivos mono/estéreo
- Configuración basada en proyectos con ajustes específicos por proyecto
- Mezcla a WAV o FLAC
- Temas oscuro y claro
- Localizado en 18 idiomas incluyendo English, Deutsch, Español, Français, Japanese, Português, Chinese, Русский, Svenska, Suomi, Dansk, Norsk, Polski, Ελληνικά, Gaeilge, Cymraeg y Pirate
- Aplicación complementaria Virtual MIDI Keyboard

## Primeros Pasos

### Iniciar la Aplicación

Ejecute el archivo `musicians_canvas` desde el directorio de compilación o su ubicación de instalación:

```
./musicians_canvas
```

Al iniciar por primera vez, la aplicación se abre con un proyecto vacío. Necesitará establecer un directorio de proyecto antes de grabar.

Al iniciar, la aplicación aplica el tema guardado (oscuro o claro) y, si un directorio de proyecto fue utilizado previamente y contiene un archivo `project.json`, el proyecto se carga automáticamente.

### Configurar un Proyecto

1. **Establecer el directorio del proyecto**: Ingrese o navegue hasta una carpeta en el campo "Project Location" en la parte superior de la ventana. Aquí es donde se almacenarán las grabaciones y el archivo del proyecto.

2. **Agregar una pista**: Haga clic en el botón **+ Add Track**. Aparecerá una nueva pista con la configuración predeterminada. Si es la única pista del proyecto y aún no ha sido grabada, se arma automáticamente para grabación.

3. **Nombrar la pista**: Escriba un nombre en el campo de texto junto al botón "Options". Este nombre se utiliza como nombre del archivo de audio grabado.

![Ventana principal](../screenshots/MusiciansCanvas_1_MainWin.png)

### Barra de botones

Justo debajo de la barra de menús hay una barra de herramientas con botones de acceso rápido:

![Barra de botones](../screenshots/ButtonBar.png)

- **Open Project**: Igual que **File > Open Project** — abre un proyecto guardado previamente.
- **Save Project**: Igual que **File > Save Project** — guarda el proyecto actual. Este
  botón solo está habilitado cuando se ha establecido un directorio de proyecto.
- **Project Settings**: Igual que **Project > Project Settings** — abre el diálogo de ajustes
  del proyecto. Este botón solo está habilitado cuando se ha establecido un directorio de proyecto.
- **Configuration**: Igual que **Settings > Configuration** — abre el diálogo de ajustes globales
  de la aplicación.
- **Metronome Settings**: Abre el diálogo de ajustes del metrónomo (vea la sección Metrónomo más abajo).

### Guardar y Abrir Proyectos

- **Guardar**: Use **File > Save Project** (Ctrl+S) para guardar el proyecto actual como un archivo JSON en el directorio del proyecto.
- **Abrir**: Use **File > Open Project** (Ctrl+O) para cargar un proyecto guardado previamente.

El archivo del proyecto (`project.json`) almacena nombres de pistas, tipos, notas MIDI, referencias a archivos de audio y todos los ajustes específicos del proyecto. Los archivos de audio se almacenan en el mismo directorio que `project.json` y se nombran según sus pistas (por ejemplo, `My_Track.flac`).

Si cierra la aplicación con cambios sin guardar, un diálogo de confirmación le preguntará si desea guardar antes de salir.

## Gestión de Pistas

### Agregar y Eliminar Pistas

- Haga clic en **+ Add Track** para agregar una nueva pista al arreglo.
- Haga clic en el botón **x** en el lado derecho de una fila de pista para eliminarla.
- Haga clic en **Clear Tracks** (el botón rojo en la barra de herramientas) para eliminar todas las pistas. Se muestra un diálogo de confirmación antes de proceder.

### Agregar Pistas Arrastrando y Soltando

Mientras un proyecto está abierto, puede arrastrar uno o más archivos de audio
compatibles desde su administrador de archivos (Explorador de Windows, Finder
de macOS, administrador de archivos de Linux, etc.) directamente a la ventana
de Musician's Canvas para agregarlos como nuevas pistas de audio.

- **Formatos compatibles:** `.wav` y `.flac`. Los archivos en cualquier otro
  formato se omiten silenciosamente, y al final un diálogo enumera qué
  archivos se omitieron.
- **Copia de archivos:** Si el archivo soltado aún no está en el directorio del
  proyecto, se copia allí automáticamente. Si ya existe un archivo con el mismo
  nombre en el directorio del proyecto, se le preguntará si desea reemplazarlo.
- **Nombre de la pista:** El nombre base del archivo (sin la extensión) se usa
  como nombre de la nueva pista. Por ejemplo, soltar `Bass Line.wav` crea una
  pista de audio llamada "Bass Line".
- **Varios archivos a la vez:** Se pueden arrastrar varios archivos juntos;
  cada archivo compatible se convierte en su propia pista en un solo gesto.
- **Cuando se rechaza el soltado:** Los arrastres solo se aceptan mientras un
  proyecto está abierto y Musician's Canvas **no** está reproduciendo ni
  grabando. Detenga la reproducción o la grabación primero si desea arrastrar
  pistas adicionales.

### Configurar el Tipo de Pista

Cada pista puede configurarse como **Audio** (para grabación desde micrófono/entrada de línea) o **MIDI** (para grabación desde teclado/controlador).

Para cambiar el tipo de pista:

- Haga clic en el botón **Options** de la pista, o
- Haga clic en el **ícono de tipo de pista** (entre "Options" y el campo de nombre)

Esto abre el diálogo de Configuración de Pista donde puede seleccionar la fuente de entrada.

![Configuración de pista](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Controles de Pista

Cada fila de pista proporciona los siguientes controles:

- **Casilla de habilitación**: Alterna si la pista se incluye en la reproducción y la mezcla. Deshabilitar una pista también la desarma automáticamente si estaba armada.
- **Botón de armar (radio)**: Selecciona esta pista como destino de grabación. Solo una pista puede estar armada a la vez; armar una nueva pista desarma automáticamente cualquier pista previamente armada.
- **Campo de nombre**: Campo de texto editable para el nombre de la pista. Este nombre se usa como nombre del archivo de audio (los caracteres no válidos para el sistema de archivos se reemplazan por guiones bajos).
- **Botón Options**: Abre el diálogo de Configuración de Pista.
- **Ícono de tipo**: Muestra un ícono de altavoz para pistas de audio o un ícono de piano para pistas MIDI. Al hacer clic se abre el diálogo de Configuración de Pista.
- **Botón de eliminar (x)**: Elimina la pista del proyecto.

### Armado Automático

Cuando un proyecto tiene exactamente una pista y esa pista aún no ha sido grabada, se arma automáticamente para grabación. Esto aplica tanto al agregar la primera pista a un proyecto nuevo como al abrir un proyecto existente que tiene una única pista vacía.

### Visualización de Pistas

- **Pistas de audio** muestran una visualización de forma de onda del audio grabado. Cuando no se ha grabado audio, el área muestra "No audio recorded".
- **Pistas MIDI** muestran una visualización de piano roll que presenta las notas grabadas en una cuadrícula que abarca de A0 a C8. Las notas se colorean según la velocidad. Cuando no se han grabado datos MIDI, el área muestra "No MIDI data recorded".

## Grabación

### Grabación de Audio

1. Asegúrese de que el directorio del proyecto esté establecido.
2. Arme la pista destino (seleccione el botón de radio "Arm").
3. Haga clic en el botón **Record** (círculo rojo).
4. Aparece una cuenta regresiva de 3 segundos en la pista ("Get ready... 3", "2", "1"), luego comienza la grabación.
5. Durante la grabación, se muestra un medidor de nivel en vivo en el área de forma de onda de la pista, mostrando la amplitud actual como una barra de gradiente (verde a amarillo a rojo) con la etiqueta "Recording".
6. Haga clic en el botón **Stop** para finalizar la grabación.

El audio grabado se guarda como un archivo FLAC en el directorio del proyecto, nombrado según la pista.

Durante la grabación y la reproducción, todos los controles interactivos (botones de pista, ajustes, etc.) se deshabilitan para prevenir cambios accidentales.

### Efectos de inserción (solo pistas de audio)

Las pistas de audio tienen un botón **Efectos** justo debajo de **Options**. Abre el diálogo
**Efectos de pista**, donde puede definir una **cadena ordenada** de efectos insertados para las
grabaciones en esa pista:

![Diálogo Efectos de pista](../screenshots/Track_Effects_Dialog.png)

Puede usar **Reverb**, **Chorus**, **Flanger**, **Overdrive / distorsión**, **Amp & cabinet** (modelado de amplificador y pantalla: tipo de ampli, pantalla, ganancia, graves/medios/agudos, aire y mezcla) y **Vibrato (Trémolo)**.

**Vibrato (Trémolo)** es el clásico “Vibrato” de Fender (en realidad un trémolo): un **cambio oscilante de volumen con el tiempo**. Use **Speed** para ajustar la velocidad del pulso y **Intensity** (Depth) para ajustar la profundidad (de un movimiento sutil a un “chop” completo).

Capturas del efecto **Amp & cabinet**:

![Modelado Amp & cabinet (1)](../screenshots/Amp_And_Cabinet_Model_1.png)

![Modelado Amp & cabinet (2)](../screenshots/Amp_And_Cabinet_Model_2.png)

- Pulse **Añadir efecto…** y elija un tipo de efecto. Puede añadir varias instancias y
  quitar una con la **✕** roja en la cabecera del efecto.
- Arrastre el control **≡** para **reordenar**. El efecto de **arriba** se aplica **primero**.
- Los controles usan milisegundos y hercios, coherentes tras la conversión a la **frecuencia de
  muestreo del proyecto**. Se admite **mono** y **estéreo** (el mono se procesa como dual-mono y
  se mezcla de nuevo a un canal).
- **OK** guarda los cambios en el proyecto; **Cancel** restaura la cadena al abrir el diálogo.

Los efectos se aplican al **detener la grabación**, después del flujo normal de captura y
remuestreo. La configuración se guarda en `project.json` en `audioEffectChain`.

### Mix effects (full project)

**Project → Project Settings → Mix Effects** lets you build the same kind of ordered effect chain as **Track effects** (**Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet**, **Vibrato (Tremolo)**), but applied to the **entire mixed program**: when you press **Play** to hear all enabled tracks together, and when you export with **Mix tracks to file** (toolbar or **Tools** menu). The chain is saved in `project.json` under `projectSettings` → `mixEffectChain`.

**Project → Project Settings → Aux / Send Bus** configura una **cadena de efectos compartida** alimentada por el control de envío **Aux** de cada pista (en la fila de la pista). Se suma la mezcla seca de todas las pistas; la señal tras **Gain** y **Pan** se escala con el nivel **Aux** y pasa por este bus; la salida **húmeda** del aux se suma de nuevo a la suma seca **antes** de **Mix Effects**. Sirve para un reverb o delay compartido manteniendo los inserts por pista.

To reduce harsh [digital clipping](https://en.wikipedia.org/wiki/Clipping_%28audio%29) when processing pushes peaks toward full scale, the effect engine applies a **soft limiter** to normalized float samples immediately before conversion to 16-bit PCM. The **EffectWidget** base class documents `guardFloatSampleForInt16Pcm()` and `softLimitFloatSampleForInt16Pcm()` for any new real-time code that writes to 16-bit audio.

### Mezclador por pista, mute/solo, recorte y MIDI

Cada fila de pista incluye una franja de **mezclador**:

- **Gain**: Nivel en decibelios (el deslizador usa décimas de dB; 0 dB = unidad).
- **Pan**: Panorámica (-100 = todo a la izquierda, +100 = todo a la derecha).
- **Aux**: Cantidad de envío (0–100 %) al **Aux / Send Bus** del proyecto (arriba).
- **Mute**: Silencia la pista en la mezcla sin desactivarla en la vista de arreglo.
- **Solo**: Si **alguna** pista tiene **Solo**, solo se oyen las pistas en solo (salvo que también estén en mute).

**Options → Track Configuration** ofrece **Clip trim (no destructivo)**: **Trim start** y **Trim end** omiten esos segundos al inicio y al final del clip para **reproducción, mezcla y exportación** sin borrar la grabación subyacente.

Las pistas MIDI pueden llevar automatización **CC** en el proyecto y en `.mid` exportados; la reproducción y mezcla offline usan esos eventos al renderizar MIDI a audio.

**Edit → Undo** / **Redo** (atajos habituales) aplican a cambios de mezclador y recorte en las pistas.

### Monitorizar durante la grabación

Junto al **visor de tiempo**, la casilla **Escuchar el audio mientras se graba** decide si se envía la **entrada en directo** a la **salida de audio del proyecto** durante la grabación:

- **Pistas de audio**: se reproduce en tiempo real lo que entra (la grabación sigue igual). Se suma a la reproducción de **overdub** si hay otras pistas.
- **Pistas MIDI**: si el proyecto **renderiza MIDI a audio para reproducción** y hay **SoundFont**, oirá las notas por el sintetizador integrado. Con **salida MIDI externa**, use el monitor de su equipo.

La opción se **guarda en el proyecto** (`monitorWhileRecording` en `project.json`). Desactívela para evitar retroalimentación por micrófono.

#### Grabación en Overdub

Al grabar una nueva pista mientras otras pistas habilitadas ya contienen datos de audio o MIDI, Musician's Canvas realiza una grabación en overdub: las pistas existentes se mezclan y se reproducen en tiempo real mientras se graba la nueva pista. Esto le permite escuchar las partes previamente grabadas mientras graba una nueva.

La mezcla de las pistas existentes se prepara antes de que comience la captura, de modo que la grabación y la reproducción inician aproximadamente en el mismo instante, manteniendo todas las pistas sincronizadas.

#### Backends de Grabación

Musician's Canvas soporta dos backends de captura de audio:

- **PortAudio** (predeterminado cuando está disponible): Proporciona grabación confiable y de baja latencia. Es el backend recomendado.
- **Qt Multimedia**: Un backend alternativo que utiliza la captura de audio integrada de Qt. Se usa cuando PortAudio no está disponible o cuando se selecciona explícitamente en Project Settings.

El backend de grabación puede configurarse por proyecto en **Project > Project Settings > Audio**.

#### Frecuencia de Muestreo y Manejo de Dispositivos

Musician's Canvas graba a la frecuencia de muestreo nativa del dispositivo de entrada de audio y luego convierte automáticamente a la frecuencia de muestreo configurada del proyecto utilizando remuestreo de alta calidad. Esto significa que puede establecer cualquier frecuencia de muestreo del proyecto (por ejemplo, 44100 Hz o 48000 Hz) independientemente de la frecuencia nativa del dispositivo. La conversión preserva el tono y la duración exactamente.

#### Detección de Dispositivos Mono

Algunos dispositivos de audio (por ejemplo, micrófonos de cámaras web USB) son físicamente mono pero el sistema operativo los presenta como estéreo. Musician's Canvas detecta esto automáticamente y ajusta el número de canales en consecuencia. Si el proyecto está configurado para estéreo, la señal mono se duplica en ambos canales.

### Grabación MIDI

1. Establezca el tipo de pista como **MIDI** mediante el botón Options.
2. Asegúrese de que un dispositivo de entrada MIDI esté configurado en **Settings > Configuration > MIDI**.
3. Arme la pista y haga clic en Record.
4. Toque notas en su controlador MIDI.
5. Haga clic en Stop para finalizar la grabación.

Las notas MIDI se muestran en una visualización de piano roll en la pista.

## Metrónomo

Musician's Canvas incluye un metrónomo integrado que puede usarse durante la grabación para
ayudar a mantener el tiempo. Haga clic en el botón del metrónomo en la barra de botones
(debajo de la barra de menús) para abrir el diálogo de ajustes del metrónomo:

![Ajustes del metrónomo](../screenshots/MetronomeSettings.png)

El diálogo proporciona:

- **Enable metronome during recording**: Cuando está marcado, el metrónomo reproduce un
  sonido de tic mientras la grabación está activa. El tic se reproduce a través del audio del
  sistema y **no** se captura en la pista grabada.
- **Beats per minute**: Una entrada numérica para el tempo, en pulsos por minuto (BPM). El
  rango es de 20 a 300 BPM. Si **Project → Tempo map** define cambios de tempo, el metrónomo sigue esos marcadores durante la grabación (el campo BPM sigue fijando el tempo inicial mientras no aplique un marcador).

Cuando el metrónomo está habilitado, comienza a sonar una vez que la grabación realmente
comienza (después de que termina la cuenta regresiva de 3 segundos) y se detiene cuando
finaliza la grabación.

## Reproducción

Haga clic en el botón **Play** para mezclar y reproducir todas las pistas habilitadas. El tooltip del botón cambia para indicar si reproducirá o grabará según si hay una pista armada. Las pistas deshabilitadas (desmarcadas) se excluyen de la reproducción.

Durante la reproducción, las pistas de audio se decodifican desde sus archivos FLAC y las pistas MIDI se renderizan a audio utilizando el sintetizador FluidSynth integrado. Todas las pistas se mezclan y se reproducen a través del dispositivo de salida de audio del sistema.

Haga clic en el botón **Stop** para detener la reproducción en cualquier momento.

## Mezcla a Archivo

Use **Tools > Mix tracks to file** (Ctrl+M) para exportar todas las pistas habilitadas a un único archivo de audio. Un diálogo le permite elegir la ruta de salida y el formato:

- **Archivo de salida**: Navegue para seleccionar la ruta del archivo de destino.
- **Formato**: Elija entre FLAC (compresión sin pérdida, archivos más pequeños) o WAV (sin comprimir).

La mezcla utiliza la frecuencia de muestreo configurada del proyecto. Las pistas MIDI se renderizan usando el SoundFont configurado. Se aplican **Gain**, **Pan**, **mute/solo**, **envío Aux** y **trim** por pista igual que en reproducción.

**Tools → Export stems to folder** escribe un archivo **WAV** por pista (pistas habilitadas que entren en la mezcla). Cada stem refleja el mezclador y trim de esa pista; los **Mix Effects** maestros **no** se aplican a stems individuales.

## Ajustes

### Ajustes Globales

Use **Settings > Configuration** (Ctrl+,) para establecer los valores predeterminados globales que se aplican a todos los proyectos:

![Ajustes generales](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Pestaña General

- **Tema**: Elija entre los temas Oscuro y Claro.
- **Escribir registro de depuración de grabación**: Cuando está habilitado, Musician's
  Canvas escribe información detallada de diagnóstico de grabación en un archivo llamado
  `recording_debug.txt` en el directorio del proyecto. Esto incluye el backend de captura,
  el nombre del dispositivo, la frecuencia de muestreo, el número de canales y datos de
  temporización. Esta opción está deshabilitada por defecto y es principalmente útil para
  solucionar problemas de grabación.

#### Pestaña Display

- **Color del LED de visualización numérica**: Elija el color usado para la pantalla
  numérica LED de tiempo mostrada en la barra de herramientas de la ventana principal.
  Los segmentos activos de los dígitos se dibujan con el color elegido, y los segmentos
  inactivos se dibujan como una versión atenuada del mismo color. Los colores
  disponibles son Light Red, Dark Red, Light Green, Dark Green, Light Blue, Dark Blue,
  Yellow, Orange, Light Cyan y Dark Cyan. El valor predeterminado es Light Green.

![Configuración de pantalla](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Pestaña Language

- **Idioma**: Elija el idioma de la interfaz de la aplicación. El valor predeterminado es
  "System Default", que utiliza la configuración de idioma del sistema operativo. Los
  idiomas disponibles son English, Deutsch (German), Español (Spanish), Français (French),
  Japanese, Português (Brazilian Portuguese), Chinese (Traditional), Chinese (Simplified),
  Dansk (Danish), Ελληνικά (Greek), Gaeilge (Irish), Norsk (Norwegian), Polski (Polish),
  Русский (Russian), Suomi (Finnish), Svenska (Swedish), Cymraeg (Welsh) y Pirate. La
  interfaz se actualiza inmediatamente al cambiar el idioma.

![Configuración de idioma](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### Pestaña MIDI

- **Dispositivo de Salida MIDI**: Seleccione el sintetizador FluidSynth integrado o un dispositivo MIDI externo. Use el botón **Refresh** para volver a buscar dispositivos MIDI disponibles.
- **SoundFont**: Navegue hasta un archivo SoundFont `.sf2` para la síntesis MIDI. En Linux, un SoundFont del sistema puede ser detectado automáticamente si el paquete `fluid-soundfont-gm` está instalado. En Windows y macOS, debe configurar la ruta del SoundFont manualmente.

![Ajustes MIDI](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Pestaña Audio

- **Dispositivo de Entrada de Audio**: Seleccione el micrófono o dispositivo de entrada de línea para grabación.
- **Dispositivo de Salida de Audio**: Seleccione el altavoz o auriculares para reproducción.

![Ajustes de audio](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Ajustes del Proyecto

Use **Project > Project Settings** (Ctrl+P) para anular los valores predeterminados globales solo para el proyecto actual. Esto es útil para proyectos que necesitan una frecuencia de muestreo, SoundFont o dispositivo de audio específico. Los ajustes específicos del proyecto se guardan dentro del archivo `project.json`.

![Ajustes MIDI del proyecto](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Pestaña MIDI

- **Dispositivo MIDI**: Seleccione un dispositivo MIDI para este proyecto, o déjelo en el valor predeterminado para usar el ajuste global.
- **SoundFont**: Seleccione un archivo SoundFont para este proyecto.
- **Refresh**: Volver a buscar dispositivos MIDI disponibles.

#### Pestaña Audio

- **Dispositivo de Entrada de Audio**: Seleccione el dispositivo de grabación para este proyecto.
- **Backend de captura de grabación** (cuando PortAudio está disponible):
  - **PortAudio (native input)**: Recomendado. Usa la misma biblioteca de audio que Audacity.
  - **Qt Multimedia**: Opción alternativa que utiliza la captura de audio integrada de Qt.
- **Dispositivo de Entrada PortAudio**: Al usar el backend PortAudio, seleccione el dispositivo de entrada PortAudio específico.
- **Dispositivo de Salida de Audio**: Seleccione el dispositivo de reproducción para este proyecto.

##### Ajustes de Formato de Audio

- **Frecuencia de muestreo**: Elija entre las frecuencias estándar (8000 Hz a 192000 Hz). La frecuencia nativa del dispositivo se etiqueta como "(native)". Las frecuencias que requieren remuestreo se etiquetan como "(resampled)". Puede seleccionar cualquier frecuencia independientemente de la capacidad del dispositivo; Musician's Canvas realizará el remuestreo automáticamente según sea necesario.
- **Canales**: Mono o Estéreo. Si el dispositivo solo soporta mono, la opción Estéreo se deshabilita.

![Ajustes de audio del proyecto](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

#### Mix Effects tab

The **Mix Effects** tab is a scrollable list with the same controls as **Track effects** (**Add effect…**, drag **≡** to reorder, **✕** to remove). Processing order is **top to bottom** on the **combined** mix of all enabled tracks. These effects run during **whole-project playback** and when **mixing to a single WAV or FLAC file**; they are **not** baked into individual track files on disk. An empty list leaves the mixed signal unchanged aside from the mixer's own level handling.

#### Pestaña Aux / Send Bus

Configura la **cadena de efectos aux compartida** (mismos tipos que inserts de pista). El deslizador **Aux** de cada pista controla cuánta señal entra en este bus; la vuelta húmeda se suma a la mezcla seca **antes** de **Mix Effects**.

## Menús

### Menú File

| Elemento del Menú | Atajo    | Descripción                              |
|--------------------|----------|------------------------------------------|
| Save Project       | Ctrl+S   | Guardar el proyecto actual en disco      |
| Open Project       | Ctrl+O   | Abrir un archivo de proyecto existente   |
| Close              | Ctrl+Q   | Cerrar la aplicación                     |

### Menú Project

| Elemento del Menú                   | Atajo    | Descripción                                        |
|--------------------------------------|----------|----------------------------------------------------|
| Project Settings                     | Ctrl+P   | Configurar ajustes del proyecto (incl. **Aux / Send Bus**)        |
| Tempo map                           |          | Editar cambios de tempo (s vs BPM) para metrónomo y cuantización MIDI |
| Add Demo Data to Selected Track      |          | Agregar notas MIDI de ejemplo para demostración    |

### Menú Edit

| Elemento del Menú | Atajo    | Descripción                                    |
|--------------------|----------|-----------------------------------------------|
| Undo               | Ctrl+Z   | Deshacer cambios de mezclador/trim en pistas |
| Redo               | Ctrl+Shift+Z | Rehacer                                  |

### Menú Settings

| Elemento del Menú | Atajo    | Descripción                                    |
|--------------------|----------|------------------------------------------------|
| Configuration      | Ctrl+,   | Abrir ajustes globales de la aplicación        |

### Menú Tools

| Elemento del Menú     | Atajo    | Descripción                                        |
|------------------------|----------|----------------------------------------------------|
| Mix tracks to file     | Ctrl+M   | Exportar todas las pistas habilitadas a un archivo |
| Export stems to folder |          | Un WAV por pista (gain/pan/trim; sin Mix Effects maestros) |
| Recording options |          | Región **punch-in**; **loop** de reproducción del proyecto |
| Quantize MIDI         |          | Alinear inicios de notas MIDI a rejilla (todas o pista armada) |
| Add drum track        | D        | Añadir pista MIDI de batería y escribir un `.mid` (ver abajo) |
| Virtual MIDI Keyboard  |          | Iniciar la aplicación complementaria de teclado    |

### Menú Help

| Elemento del Menú | Atajo | Descripción |
|-------------------|-------|-------------|
| Manual            | Alt+M | Abre el manual PDF en el idioma seleccionado actualmente |
| About             |       | Muestra información de versión y de la aplicación |

## Atajos de Teclado

| Atajo           | Acción                                |
|-----------------|---------------------------------------|
| Ctrl+S          | Guardar proyecto                      |
| Ctrl+O          | Abrir proyecto                        |
| Ctrl+Z          | Deshacer (mezclador/trim)           |
| Ctrl+Shift+Z    | Rehacer                             |
| Ctrl+M          | Mezclar pistas a archivo              |
| D               | Añadir pista de batería (menú Tools)   |
| Ctrl+P          | Ajustes del Proyecto                  |
| Ctrl+,          | Ajustes / Configuración               |
| Alt+M           | Abrir manual PDF (Help)               |
| Ctrl+Q / Alt+F4 | Salir                                |


### Añadir pista de batería

**Tools → Add drum track** (atajo **D**) crea una pista **MIDI** de percusión en el **canal 10** General MIDI (índice de canal 9). El nombre predeterminado suele ser **Drums** (con sufijo numérico si hace falta).

Se escribe de inmediato un **`.mid`** en la **carpeta del proyecto**: dos compases 4/4 con bombo, caja y charleston cerrado. El tempo:

- Si en **Ajustes del metrónomo** está **Activar metrónomo durante la grabación**, se usa ese **BPM**.
- Si no, se **estima el BPM** a partir de las pistas de **audio** **habilitadas** con contenido; si no hay audio útil, **120 BPM**.

**Nota:** la detección es sencilla; puede confundir tiempos relacionados por 2.

**Referencias:** [Audient — batería MIDI realista](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/), [Melda MDrummer](https://www.meldaproduction.com/MDrummer), [Reddit](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/), [PDF Stanford CS229](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf).

## Virtual MIDI Keyboard

Virtual MIDI Keyboard es una aplicación complementaria (`virtual_midi_keyboard`) que proporciona un teclado de piano en pantalla para enviar notas MIDI. Puede iniciarse desde el menú **Tools > Virtual MIDI Keyboard** en la aplicación principal, o ejecutarse de forma independiente.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Funciones

- Haga clic en las teclas del piano en pantalla para tocar notas
- Use el teclado de la computadora como un teclado de piano (vea la asignación de teclas a continuación)
- Conéctese a dispositivos de salida MIDI externos o use el sintetizador FluidSynth integrado
- Conéctese a un dispositivo de entrada MIDI para mostrar las notas entrantes en el teclado
- Volumen del sintetizador ajustable (ganancia maestra, 10%--200%)
- Selección de SoundFont para el sintetizador integrado
- Selección de instrumento/programa MIDI (General MIDI o presets del SoundFont)
- Perilla de control de coro/efecto
- Cambio de octava (-3 a +5)
- Control de volumen MIDI (CC#7, 0--127)

### Controles de la Barra de Herramientas

- **Volume**: Volumen MIDI (CC#7), ajustable de 0 a 127 mediante el cuadro numérico.
- **Octave**: Cambie la octava del teclado usando los botones **<** y **>** o el cuadro numérico. El rango es de -3 a +5.
- **Chorus/Effect**: Una perilla rotativa y un campo de texto (1--127) para establecer el nivel de coro/efecto (MIDI CC#93). Haga clic en **Apply** para enviar el valor.
- **Canal MIDI**: Una entrada numérica (1--16) selecciona el canal MIDI por el que se envían las notas, los cambios de programa y los cambios de control. El valor predeterminado es el canal 1. El canal seleccionado entra en vigor inmediatamente al cambiarlo. Al elegir un preset de SoundFont mientras hay un canal determinado seleccionado, se aplica ese bank/program a ese canal, de modo que puede tocar sonidos diferentes en canales diferentes.
- **Batería**: Una casilla de verificación situada justo encima del selector de canal MIDI. Cuando no está marcada (opción predeterminada), el canal MIDI es el que el usuario haya elegido. Cuando está marcada, el canal MIDI se cambia automáticamente al canal 10 — el canal de batería estándar de General MIDI — de modo que lo que se toque suene como percusión. Al desmarcar la casilla se restaura el canal que estaba seleccionado antes de activar Batería.
- **Selector de instrumento**: Elija un instrumento MIDI. Al usar el sintetizador FluidSynth integrado, muestra los presets del SoundFont (Bank:Program Name). Al conectarse a un dispositivo MIDI externo, lista los 128 instrumentos General MIDI.

### Tocar con el Teclado de la Computadora

El teclado de la computadora está asignado a teclas de piano a lo largo de dos octavas:

**Octava inferior (comenzando en la octava actual):**

| Tecla | Nota  |
|-------|-------|
| Z     | C     |
| S     | C#/Db |
| X     | D     |
| D     | D#/Eb |
| C     | E     |
| V     | F     |
| G     | F#/Gb |
| B     | G     |
| H     | G#/Ab |
| N     | A     |
| J     | A#/Bb |
| M     | B     |

**Octava superior (una octava más alta):**

| Tecla | Nota  |
|-------|-------|
| Q     | C     |
| 2     | C#/Db |
| W     | D     |
| 3     | D#/Eb |
| E     | E     |
| R     | F     |
| 5     | F#/Gb |
| T     | G     |
| 6     | G#/Ab |
| Y     | A     |
| 7     | A#/Bb |
| U     | B     |
| I     | C (siguiente octava) |
| 9     | C#/Db |
| O     | D     |
| 0     | D#/Eb |
| P     | E     |

Las teclas se iluminan visualmente al presionarlas (las teclas blancas se vuelven celestes, las teclas negras se oscurecen).

### Configuración

Abra el diálogo de Configuración (**Settings > Configuration**, Ctrl+,) para configurar los dispositivos MIDI y de audio:

![Configuración de Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard2.png)

#### Pestaña MIDI

- **Dispositivo de Salida MIDI**: Seleccione un dispositivo externo o déjelo en blanco para usar el sintetizador FluidSynth integrado.
- **Dispositivo de Entrada MIDI**: Seleccione un controlador para reenviar notas a la visualización del teclado.
- **Volumen del Sintetizador (Ganancia Maestra)**: Ajuste el nivel de salida del sintetizador integrado (10%--200%).
- **SoundFont**: Navegue hasta un archivo SoundFont `.sf2` para el sintetizador integrado.
- **Refresh**: Volver a buscar dispositivos MIDI disponibles.

#### Pestaña Audio

- **Dispositivo de Salida de Audio**: Seleccione el dispositivo de salida para el sintetizador integrado.

#### Pestaña Language

- **Idioma**: Elija el idioma de la interfaz. Los mismos idiomas que Musician's Canvas
  están disponibles. La interfaz se actualiza inmediatamente al cambiar el idioma.

### Atajos de Virtual MIDI Keyboard

| Atajo    | Acción                         |
|----------|--------------------------------|
| Ctrl+,   | Diálogo de configuración       |
| Ctrl+U   | Ayuda / Información de uso     |
| Ctrl+Q   | Cerrar                         |

## FAQ (Preguntas frecuentes)

### ¿Cómo grabo una pista MIDI?

1. Establezca un **directorio de proyecto** (necesario para guardar).
2. Añada o seleccione una pista y abra **Options** (o haga clic en el icono de tipo de pista).
3. Configure el tipo de pista como **MIDI** y cierre el cuadro de diálogo.
4. En **Settings > Configuration > MIDI**, elija la **entrada MIDI** (puerto físico o cable virtual) y un **SoundFont** (`.sf2`) para la reproducción posterior.
5. **Arme** esa pista (solo una pista puede estar armada a la vez).
6. Pulse **Record**, espere la cuenta atrás, toque el controlador y luego **Stop**.

Las notas aparecen en el piano roll. **File > Save Project** escribe un archivo `.mid` (y `project.json`) en la carpeta del proyecto.

### ¿Por qué mi pista MIDI no suena al reproducir?

La reproducción usa **FluidSynth** con el **SoundFont** de la configuración. Revise **Settings > Configuration > MIDI** (o **Project > Project Settings** si hay anulaciones por proyecto): debe haber una ruta `.sf2` válida. En Linux a veces se detecta un SoundFont del sistema; en Windows y macOS suele ser necesario elegir manualmente un archivo SoundFont.

### ¿Cómo se relaciona Virtual MIDI Keyboard con Musician's Canvas?

Son **programas independientes**. Inicie Virtual MIDI Keyboard desde **Tools > Virtual MIDI Keyboard** (o por separado). Para que el teclado en pantalla **entre** en Musician's Canvas mientras graba una pista MIDI, el sistema debe enrutar la **salida MIDI** del teclado a una **entrada** que use Musician's Canvas — a menudo con un cable MIDI virtual o eligiendo puertos coherentes en ambas aplicaciones. No se conectan solas.

### ¿Qué diferencia hay entre Configuration y Project Settings?

**Settings > Configuration** define los **valores predeterminados globales** (tema, idioma, dispositivos MIDI/audio, SoundFont, etc.) de la aplicación. **Project > Project Settings** sobrescribe parte de eso **solo para el proyecto actual** y se guarda en `project.json`. Si un campo queda en el predeterminado del proyecto, se usa el valor global de Configuration.

### ¿Por qué no funciona arrastrar y soltar archivos de audio?

Solo se aceptan archivos si hay **directorio de proyecto** y Musician's Canvas **no** está reproduciendo ni grabando. Los formatos admitidos son **`.wav`** y **`.flac`**; otros se omiten y se listan en un diálogo. Cada archivo crea una nueva pista **audio** con el nombre del archivo sin extensión.

### ¿Dónde se guardan mis grabaciones?

El audio se guarda en el **directorio del proyecto** como **`<nombre_pista>.flac`** (caracteres como `/ \ : * ? " < > |` se sustituyen por guiones bajos). El archivo de proyecto es **`project.json`** en la misma carpeta. Las pistas MIDI se guardan como **`<nombre_pista>.mid`** al guardar el proyecto (junto con las notas en `project.json`).

### ¿Qué nombre de archivo se usa al renderizar MIDI para mezclar?

Al **mezclar** o **reproducir**, el MIDI se renderiza primero a un WAV temporal. Si se conoce la ruta del proyecto, Musician's Canvas también escribe un **FLAC en caché** en la carpeta del proyecto: **`<nombre_pista_sanitizado>.flac`** (misma regla de saneamiento que otros archivos de pista). El nombre sigue el **nombre de pista**, no un ID interno.

### ¿Puedo grabar dos pistas a la vez?

No. Solo **una** pista puede estar **armada**; recibe la siguiente grabación. Compone grabando **una tras otra** (el **overdub** reproduce las pistas existentes mientras graba una nueva).

### ¿Se graba el metrónomo en la pista?

No. Si está activado, el metrónomo suena por el **audio del sistema** solo como referencia. **No** se mezcla en el archivo grabado.

### ¿Por qué Musician's Canvas pide ASIO en Windows?

En Windows la aplicación principal espera un controlador **ASIO** para audio fiable y de baja latencia. Instale **ASIO4ALL** o el controlador del fabricante de su interfaz si aparecen errores.

### En macOS, ¿dónde está Virtual MIDI Keyboard?

En el **bundle `.app`**, el ejecutable de Virtual MIDI Keyboard se **copia dentro de** `Musician's Canvas.app` (**Contents/MacOS/**) para poder distribuir una sola carpeta de aplicación. Sígalo abriendo **Tools > Virtual MIDI Keyboard**.

## Solución de Problemas

### Sin Salida de Audio

- Verifique que el dispositivo de salida de audio correcto esté seleccionado en Settings > Configuration > Audio.
- En Linux, verifique que PipeWire o PulseAudio esté ejecutándose y que la salida no esté silenciada. Use `amixer` o los ajustes de sonido de su escritorio para verificar los niveles de volumen.
- En Windows, asegúrese de que un controlador de audio ASIO esté instalado (por ejemplo, [ASIO4ALL](https://asio4all.org/) o un controlador ASIO proporcionado por el fabricante de su interfaz de audio). Musician's Canvas requiere ASIO para audio de baja latencia en Windows.

### Las Pistas MIDI No Producen Sonido

- Asegúrese de que un SoundFont (archivo `.sf2`) esté configurado en Settings > Configuration > MIDI.
- En Linux, un SoundFont del sistema puede ser detectado automáticamente si el paquete `fluid-soundfont-gm` está instalado.
- En Windows y macOS, debe configurar la ruta del SoundFont manualmente.

### La Grabación Suena Distorsionada o Tiene un Tono Incorrecto

- Esto puede ocurrir cuando la frecuencia de muestreo nativa del dispositivo de entrada de audio difiere de la frecuencia configurada del proyecto. Musician's Canvas maneja esto automáticamente mediante remuestreo, pero si los problemas persisten, intente establecer la frecuencia de muestreo del proyecto para que coincida con la frecuencia nativa del dispositivo.
- Los micrófonos de cámaras web USB a menudo tienen frecuencias nativas inusuales (por ejemplo, 32000 Hz). La aplicación los detecta automáticamente.
- Si usa el backend Qt Multimedia y experimenta problemas, intente cambiar al backend PortAudio en Project Settings > Audio.

### Virtual MIDI Keyboard No Produce Audio

- En Linux con PipeWire, asegúrese de que el paquete `libpipewire-0.3-dev` esté instalado (necesario para la integración de PipeWire con el sintetizador FluidSynth).
- Verifique que un SoundFont esté cargado (vea la pestaña MIDI en el diálogo de configuración).
- Verifique que el dispositivo de salida de audio esté seleccionado y que el volumen del sistema no esté silenciado.

## Compilación desde el Código Fuente

Consulte el [README](../README.md) para obtener instrucciones completas de compilación para Linux, macOS y Windows, incluyendo todas las dependencias requeridas.
