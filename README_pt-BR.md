# Musician's Canvas

Este é um aplicativo de gravação musical multipista para PCs desktop. É escrito em C++ e usa Qt como
biblioteca de interface gráfica. Este aplicativo foi projetado para Linux, Windows e Mac OS. A facilidade de uso é uma
consideração principal de design. Este projeto começou como um experimento com Cursor (ferramenta de codificação com IA) para ver o que
ele seria capaz de criar como um aplicativo desse tipo, com uso contínuo de desenvolvimento com IA, bem
como desenvolvimento de software tradicional.

Anos atrás, fiz um programa de gravação musical multipista na faculdade (com o mesmo nome); esta é uma tentativa
de criar algo melhor.

## Recursos

- **Arranjo multipista**: Adicionar e remover múltiplas faixas para compor músicas
- **Faixas nomeadas**: Cada faixa possui um nome editável usado como nome do arquivo para o áudio gravado
- **Tipos de faixa**: Configure cada faixa para gravação de áudio (de microfone/entrada de linha) ou gravação MIDI (de teclado/controlador); clique no ícone de tipo de faixa para alterar rapidamente a fonte de entrada
- **Armamento de faixa**: Marque "Arm" em uma faixa para selecioná-la como alvo de gravação; apenas uma faixa pode ser armada por vez. Uma única faixa não gravada é automaticamente armada por conveniência
- **Contagem regressiva de gravação**: Uma contagem regressiva visual de 3 segundos antes do início da gravação, dando ao intérprete tempo para se preparar
- **Gravação overdub**: Ao gravar uma nova faixa enquanto faixas existentes estão habilitadas, as faixas existentes são mixadas e reproduzidas em tempo real para que você possa ouvi-las durante a gravação. A reprodução e a captura são sincronizadas para manter todas as faixas alinhadas
- **Feedback visual**: Exibição de forma de onda de áudio para faixas de áudio (com medidor de nível ao vivo durante a gravação), visualização de piano roll MIDI para faixas MIDI
- **Sintetizador MIDI integrado**: Renderiza faixas MIDI em áudio usando FluidSynth com um SoundFont configurável
- **Mixar para um único arquivo de áudio**: Exportar todas as faixas habilitadas para um único arquivo WAV ou FLAC mixado usando a biblioteca [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)
- **Salvar / Abrir projeto**: Serializar e restaurar o projeto completo (faixas, nomes, tipos, notas MIDI, referências de arquivos de áudio) de/para um arquivo JSON, com detecção de alterações não salvas ao sair
- **Configurações específicas do projeto**: Sobrescrever os padrões globais de MIDI e áudio por projeto (taxa de amostragem, SoundFont, dispositivo MIDI)
- **Captura PortAudio (compilação opcional)**: Quando o projeto é compilado com PortAudio (`HAVE_PORTAUDIO`), a gravação pode usar um caminho de entrada nativo PortAudio (similar em espírito ao Audacity) em vez de Qt Multimedia. **Project → Project Settings → Audio** permite escolher **PortAudio** ou **Qt Multimedia** e selecionar um dispositivo de entrada PortAudio. Se PortAudio não estiver instalado, a compilação ainda é bem-sucedida e a gravação usa apenas Qt Multimedia.
- **Conversão de taxa de amostragem de alta qualidade**: Grava na taxa nativa do dispositivo de áudio e converte para a taxa do projeto usando interpolação sinc com janela (janela Kaiser, ~96 dB de atenuação na banda de rejeição), a mesma família de algoritmos usada por Audacity / libsoxr. Isso permite gravar em qualquer taxa de amostragem do projeto (8000 Hz a 192000 Hz) independentemente da taxa nativa do dispositivo, sem alteração de tom ou duração.
- **Tratamento automático mono/estéreo**: Detecta dispositivos fisicamente mono que se anunciam como estéreo (comum com microfones de webcam USB no PipeWire) e converte entre mono e estéreo conforme necessário (duplicação ou média), seguindo a abordagem de roteamento de canais do Audacity
- **Efeitos de inserção na gravação (faixas de áudio)**: Use o botão **Effects** (abaixo de **Options** em cada faixa de áudio) para abrir o diálogo **Track effects**. Adicione e configure **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** e **Amp & cabinet** (modelagem de amplificador e caixa), arraste o controle **≡** para reordenar a cadeia (o de cima roda primeiro) e salve com o projeto. Os efeitos são aplicados na captura ao parar a gravação; os parâmetros estão em unidades reais (ms, Hz) para comportamento consistente em mono ou estéreo e taxas de amostragem típicas do projeto (8 kHz–192 kHz) após normalizar a captura. Veja o [manual do usuário](docs/MusiciansCanvas_User_Manual_pt-BR.md). Tradutores podem atualizar as strings em lote via `scripts/effect_i18n.tsv` (gerado por `scripts/build_effect_tsv.py`) e `scripts/fill_effect_i18n.py` após `lupdate`.
- **Monitorar ao gravar**: A caixa **Monitorar áudio durante a gravação** (à direita do relógio) envia a entrada ao vivo para a **saída de áudio do projeto** durante a captura. **Áudio**: o que está sendo gravado; **MIDI**: com renderização para áudio e SoundFont, ouve pelo sintetizador. Salvo no **projeto** (`monitorWhileRecording`). Desligue para evitar microfonia.
- **Áudio de baixa latência**: No Windows, a detecção do driver ASIO garante áudio de baixa latência; no Linux, a prioridade de agendamento do processo é elevada para menor jitter com PipeWire / PulseAudio / ALSA
- **Virtual MIDI Keyboard**: Um aplicativo complementar para enviar notas MIDI por meio de um teclado de piano por software, com sintetizador FluidSynth integrado, ganho mestre ajustável, mapeamento de teclado do computador para piano, seleção de instrumento/programa, controle de chorus/efeito e deslocamento de oitava
- **Configuração**: Seleção de dispositivo de entrada de áudio, dispositivo MIDI e arquivo SoundFont (padrões globais e sobrescrições por projeto)
- **Tema escuro / claro**: Configurável via Settings → Configuration
- **Localização**: Ambos os aplicativos estão localizados em 18 idiomas: inglês, alemão, espanhol, francês, japonês, português (Brasil), chinês tradicional, chinês simplificado, russo, sueco, finlandês, dinamarquês, norueguês, polonês, grego, irlandês, galês e pirata. O idioma padrão segue a configuração do sistema operacional e pode ser alterado em **Settings → Configuration → Language**. O sistema padrão i18n do Qt (`QTranslator` / `.ts` / `.qm`) é utilizado, e a interface é atualizada imediatamente quando o idioma é alterado.
- **Manual do usuário**: Documentação em HTML e PDF gerada a partir de fontes Markdown, disponível em todos os idiomas suportados (veja [docs/](docs/))

## Capturas de tela

<p align="center">
	<a href="screenshots/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/MusiciansCanvas_1_MainWin.png" alt="Janela principal" width="800"></a>
	<a href="screenshots/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/MusiciansCanvas_2_TrackConfig.png" alt="Configuração de faixa" width="800"></a>
	<a href="screenshots/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_3_GeneralSettings.png" alt="Configurações gerais" width="800"></a>
	<a href="screenshots/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_4_MIDISettings.png" alt="Configurações MIDI padrão/gerais" width="800"></a>
	<a href="screenshots/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_5_AudioSettings.png" alt="Configurações de áudio padrão/gerais" width="800"></a>
	<a href="screenshots/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_DisplaySettings.png" alt="Configurações de exibição" width="800"></a>
	<a href="screenshots/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_LanguageSettings.png" alt="Configurações de idioma" width="800"></a>
	<a href="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Configurações MIDI específicas do projeto" width="800"></a>
	<a href="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Configurações de áudio específicas do projeto" width="800"></a>
	<a href="screenshots/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/Track_Effects_Dialog.png" alt="Efeitos da faixa" width="800"></a>
	<a href="screenshots/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Dependências

### Compilador

É necessário um compilador compatível com **C++17** (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (obrigatório em todas as plataformas)

A funcionalidade de mixagem e exportação FLAC depende da biblioteca
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp).
Clone-a como um diretório irmão deste repositório antes de compilar:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

A compilação CMake espera encontrá-la em `../audio_mixer_cpp` relativo à raiz deste projeto.

---

### Qt6 Multimedia (opcional, mas fortemente recomendado)

`qt6-multimedia` é opcional. Sem ele, o aplicativo ainda é compilado e executado, mas
**a gravação e reprodução de áudio ficam desabilitadas** — apenas a edição MIDI e o gerenciamento de projetos
funcionarão. Instale-o junto com as bibliotecas principais do Qt6 usando os comandos de plataforma abaixo.

---

### PortAudio (opcional)

Se a biblioteca e os cabeçalhos do PortAudio estiverem instalados, o CMake habilita **`HAVE_PORTAUDIO`** e
o vincula ao **musicians_canvas**. A gravação então usa por padrão o caminho PortAudio, a menos que
o projeto esteja configurado para **Qt Multimedia** em **Project → Project Settings → Audio**. Se PortAudio
não for encontrado, a compilação continua sem ele (o wrapper compila como stubs).

---

### SoundFont para síntese MIDI

Um arquivo SoundFont (`.sf2`) é necessário para que as faixas MIDI produzam áudio. Sem um,
as faixas MIDI serão renderizadas como silêncio. No **Linux**, o sintetizador FluidSynth integrado
detectará automaticamente um SoundFont se um estiver instalado em um caminho padrão do sistema
(veja os nomes dos pacotes abaixo). No **macOS e Windows** não há um caminho padrão do sistema,
então você deve configurar o SoundFont manualmente em
**Settings → Configuration → MIDI**.

---

### Linux — Ubuntu / Debian

```bash
sudo apt install build-essential cmake \
  qt6-base-dev qt6-multimedia-dev \
  qt6-l10n-tools \
  libfluidsynth-dev librtmidi-dev libflac-dev \
  libportaudio2 portaudio19-dev \
  libpipewire-0.3-dev \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` instala `FluidR3_GM.sf2` em `/usr/share/sounds/sf2/` e é
> detectado automaticamente na inicialização. `timgm6mb-soundfont` é uma alternativa menor.
>
> `libpipewire-0.3-dev` é necessário em sistemas baseados em PipeWire para que o Virtual MIDI
> Keyboard possa chamar `pw_init()` antes de FluidSynth criar seus recursos de áudio.
> A compilação prossegue sem ele; o indicador `HAVE_PIPEWIRE` simplesmente não é definido.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` instala `FluidR3_GM.sf2` em `/usr/share/soundfonts/` e é
> detectado automaticamente na inicialização.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` instala `FluidR3_GM.sf2` em `/usr/share/soundfonts/` e é
> detectado automaticamente na inicialização. `pipewire` normalmente já está instalado em sistemas
> Arch modernos; os cabeçalhos de desenvolvimento estão incluídos no pacote principal.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac portaudio
```

> PipeWire é um subsistema exclusivo do Linux e **não** é necessário no macOS. FluidSynth
> usará CoreAudio automaticamente por meio do backend Qt Multimedia.
>
> Baixe um SoundFont General MIDI (por exemplo,
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) ou
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> e configure seu caminho em **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** requer um **driver de áudio ASIO** para gravação e reprodução de baixa latência.
O aplicativo não será iniciado se nenhum driver ASIO for detectado.
**virtual_midi_keyboard** não requer ASIO e usa diretamente o backend WASAPI do Qt Multimedia.

**Instalando um driver ASIO para musicians_canvas (escolha um):**

| Driver | Observações |
|--------|-------------|
| [ASIO4ALL](https://asio4all.org/) | Gratuito, funciona com a maioria do hardware de áudio integrado e USB |
| Driver do fabricante | Melhor latência para interfaces de áudio dedicadas (ex: Focusrite, PreSonus, RME) |

**Cadeia de ferramentas — MSYS2 (recomendado para compilações MinGW):**

Baixe e instale o MSYS2 de <https://www.msys2.org>, depois adicione
`C:\msys64\mingw64\bin` ao seu `PATH` do sistema. Instale os pacotes necessários:

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
pacman -S mingw-w64-x86_64-portaudio
pacman -S mingw-w64-x86_64-soundfont-fluid
```

> `mingw-w64-x86_64-soundfont-fluid` instala `FluidR3_GM.sf2` em
> `C:\msys64\mingw64\share\soundfonts\` (ajuste se o MSYS2 estiver instalado em outro local).
> Como o Windows não possui um caminho de busca padrão para SoundFont, você deve configurar este caminho
> manualmente em **Settings → Configuration → MIDI** após a primeira execução.
>
> PipeWire é um subsistema exclusivo do Linux; nenhum pacote PipeWire é necessário no Windows.

Páginas de referência de pacotes:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Cadeia de ferramentas — Visual Studio 2022:**

Instale o Qt 6 por meio do [Qt Online Installer](https://www.qt.io/download) e obtenha
os binários de FluidSynth, RtMidi e libFLAC (ou compile-os a partir do código-fonte).
A compilação CMake os localizará via `find_library` / `find_path`, desde que os
diretórios apropriados estejam no `CMAKE_PREFIX_PATH`.

Um SoundFont deve ser baixado separadamente (por exemplo,
[GeneralUser GS](https://schristiancollins.com/generaluser.php) ou
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) e seu
caminho configurado em **Settings → Configuration → MIDI** após a primeira execução.

> **Nota:** `advapi32` (registro do Windows) e `winmm` (Windows Multimedia) são
> vinculados automaticamente pelo CMake; nenhuma instalação adicional é necessária para esses.

---

## Compilação

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

Se as DLLs necessárias não forem encontradas em tempo de execução, execute o script auxiliar do diretório de saída da compilação:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Geração de documentação

O manual do usuário pode ser gerado como HTML e PDF a partir da fonte Markdown:

```bash
cd docs
./generate_docs.sh          # Gerar HTML e PDF
./generate_docs.sh html     # Gerar apenas HTML
./generate_docs.sh pdf      # Gerar apenas PDF
```

**Pré-requisitos:**

- **Python (preferido):** `pip3 install markdown weasyprint`
- **Alternativa:** `pandoc` e `wkhtmltopdf` (via gerenciador de pacotes do sistema)

O HTML gerado é gravado em `docs/html/` e o PDF em `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Execução

```bash
./build/musicians_canvas
# ou
./build/virtual_midi_keyboard   # aplicativo complementar Virtual MIDI Keyboard
```

---

## Uso

### musicians_canvas

1. **Definir diretório do projeto**: Digite ou navegue até uma pasta onde as gravações e o arquivo do projeto serão armazenados
2. **Adicionar faixas**: Clique em "+ Add Track"; nomeie cada faixa no campo de texto ao lado de "Options"
3. **Configurar tipo de faixa**: Clique em "Options" em uma faixa (ou clique no ícone de tipo de faixa entre "Options" e o campo de nome) para escolher Audio ou MIDI e definir a fonte de entrada
4. **Remover uma faixa**: Clique no botão "×" no lado direito da linha da faixa
5. **Configurações globais**: Use **Settings → Configuration** para definir padrões globais:
   - Aba *General*: Tema (escuro/claro)
   - Aba *MIDI*: Dispositivo de saída MIDI padrão (sintetizador FluidSynth integrado ou um dispositivo MIDI externo) e SoundFont padrão (`.sf2`) para síntese
   - Aba *Audio*: Dispositivo de entrada/saída de áudio padrão para gravação e reprodução
6. **Configurações do projeto**: Use **Project → Project Settings** (Ctrl+P) para sobrescrever as configurações de MIDI e áudio apenas para o projeto atual (ex: uma taxa de amostragem ou SoundFont diferente por música)
7. **Gravar**: Marque "Arm" na faixa alvo e clique no botão de gravação (círculo vermelho). Apenas uma faixa pode ser armada por vez
8. **Reproduzir**: Clique no botão de reprodução para mixar e reproduzir todas as faixas habilitadas
9. **Mixar para arquivo**: Use **Tools → Mix tracks to file** (Ctrl+M) para exportar em WAV ou FLAC
10. **Salvar / Abrir**: Use **File → Save Project** (Ctrl+S) e **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Abrir configuração**: Use o botão ou menu **Configuration** (Ctrl+,) para abrir o diálogo de configurações
2. **Aba MIDI**:
   - Selecione um dispositivo de saída MIDI (um sintetizador externo de hardware/software) ou deixe em branco para usar o sintetizador FluidSynth integrado
   - Selecione um dispositivo de entrada MIDI para encaminhar notas MIDI recebidas para a exibição do teclado
   - Ajuste o **Synthesizer Volume (Master Gain)** para controlar o nível de saída do sintetizador integrado (10%–200%)
3. **Aba Audio**: Selecione o dispositivo de saída de áudio usado pelo sintetizador integrado
4. **SoundFont**: Selecione um arquivo SoundFont `.sf2` para o sintetizador integrado (detectado automaticamente no Linux se um SoundFont do sistema estiver instalado)
5. **Tocar notas**: Clique nas teclas do teclado de piano na tela, use um controlador MIDI conectado ou use o teclado do computador:
   - Oitava inferior: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = sustenidos/bemóis
   - Oitava superior: Q/W/E/R/T/Y/U/I/O/P = C até E, 2/3/5/6/7/9/0 = sustenidos/bemóis
6. **Controles da barra de ferramentas**: Ajustar volume MIDI (0–127), oitava (-3 a +5), nível de chorus/efeito e selecionar instrumentos

---

## Atalhos de teclado

**musicians_canvas:**

| Atalho | Ação |
|--------|------|
| Ctrl+S | Salvar projeto |
| Ctrl+O | Abrir projeto |
| Ctrl+M | Mixar faixas para arquivo |
| Ctrl+P | Configurações do projeto |
| Ctrl+, | Ajustes / Configuração |
| Ctrl+Q / Alt+F4 | Sair |

**virtual_midi_keyboard:**

| Atalho | Ação |
|--------|------|
| Ctrl+, | Diálogo de configuração |
| Ctrl+U | Ajuda / Informações de uso |
| Ctrl+Q | Fechar |

---

## Licença

Este projeto é fornecido como está para uso educacional e pessoal.
