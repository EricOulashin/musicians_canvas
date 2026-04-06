---
title: "Manual do Utilizador do Musician's Canvas"
subtitle: "Aplicativo de Gravacao Musical Multi-Faixa"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "pt-BR"
---

# Manual do Usuario do Musician's Canvas

## Introducao

Musician's Canvas e um aplicativo de gravacao musical multi-faixa para desktops. Ele suporta
gravacao de audio a partir de microfones e dispositivos de entrada de linha, gravacao MIDI
a partir de teclados e controladores, e mixagem de todas as faixas em um unico arquivo WAV
ou FLAC. Um aplicativo complementar, Virtual MIDI Keyboard, fornece um teclado de piano
virtual para envio de notas MIDI.

Musician's Canvas foi projetado para facilidade de uso, oferecendo recursos comumente
encontrados em estacoes de trabalho de audio digital (DAWs):

- Gravacao multi-faixa de audio e MIDI
- Gravacao overdub com reproducao sincronizada das faixas existentes
- Sintetizador MIDI FluidSynth integrado com suporte a SoundFont
- Conversao de taxa de amostragem de alta qualidade para gravacao em qualquer taxa de amostragem do projeto
- Deteccao automatica de dispositivos mono/estereo
- Configuracoes baseadas em projeto com opcoes especificas por projeto
- Mixagem para WAV ou FLAC
- Temas escuro e claro
- Localizado em 18 idiomas incluindo English, Deutsch, Español, Français, Japanese, Português, Chinese, Русский, Svenska, Suomi, Dansk, Norsk, Polski, Ελληνικά, Gaeilge, Cymraeg e Pirate
- Aplicativo complementar Virtual MIDI Keyboard

## Primeiros Passos

### Iniciando o Aplicativo

Execute o executavel `musicians_canvas` a partir do diretorio de compilacao ou do local
de instalacao:

```
./musicians_canvas
```

Na primeira execucao, o aplicativo abre com um projeto vazio. Voce precisara definir um
diretorio de projeto antes de gravar.

Na inicializacao, o aplicativo aplica o tema salvo (escuro ou claro) e, se um diretorio
de projeto foi usado anteriormente e contem um arquivo `project.json`, o projeto e
carregado automaticamente.

### Configurando um Projeto

1. **Defina o diretorio do projeto**: Insira ou navegue ate uma pasta no campo "Project
   Location" na parte superior da janela. E aqui que as gravacoes e o arquivo de projeto
   serao armazenados.

2. **Adicione uma faixa**: Clique no botao **+ Add Track**. Uma nova faixa aparece com
   configuracoes padrao. Se for a unica faixa no projeto e ainda nao tiver sido gravada,
   ela e automaticamente armada para gravacao.

3. **Nomeie a faixa**: Digite um nome no campo de texto ao lado do botao "Options". Este
   nome e usado como nome do arquivo de audio gravado.

![Janela principal](../screenshots/MusiciansCanvas_1_MainWin.png)

### Barra de Botoes

Logo abaixo da barra de menus ha uma barra de ferramentas com botoes de acesso rapido:

![Barra de botoes](../screenshots/ButtonBar.png)

- **Open Project**: O mesmo que **File > Open Project** — abre um projeto salvo anteriormente.
- **Save Project**: O mesmo que **File > Save Project** — salva o projeto atual. Este botao
  so e habilitado quando um diretorio de projeto esta definido.
- **Project Settings**: O mesmo que **Project > Project Settings** — abre o dialogo de
  configuracoes do projeto. Este botao so e habilitado quando um diretorio de projeto esta definido.
- **Configuration**: O mesmo que **Settings > Configuration** — abre o dialogo de configuracoes
  globais do aplicativo.
- **Metronome Settings**: Abre o dialogo de configuracoes do metronomo (veja a secao Metronomo abaixo).

### Salvando e Abrindo Projetos

- **Salvar**: Use **File > Save Project** (Ctrl+S) para salvar o projeto atual como um
  arquivo JSON no diretorio do projeto.
- **Abrir**: Use **File > Open Project** (Ctrl+O) para carregar um projeto salvo
  anteriormente.

O arquivo de projeto (`project.json`) armazena nomes de faixas, tipos, notas MIDI,
referencias de arquivos de audio e todas as configuracoes especificas do projeto. Os
arquivos de audio sao armazenados no mesmo diretorio que o `project.json` e sao nomeados
de acordo com suas faixas (por exemplo, `My_Track.flac`).

Se voce fechar o aplicativo com alteracoes nao salvas, um dialogo de confirmacao pergunta
se deseja salvar antes de sair.

## Gerenciamento de Faixas

### Adicionando e Removendo Faixas

- Clique em **+ Add Track** para adicionar uma nova faixa ao arranjo.
- Clique no botao **x** no lado direito da linha da faixa para remove-la.
- Clique em **Clear Tracks** (o botao vermelho na barra de ferramentas) para remover
  todas as faixas. Um dialogo de confirmacao e exibido antes de prosseguir.

### Configurando o Tipo de Faixa

Cada faixa pode ser configurada como **Audio** (para gravacao de microfone/entrada de
linha) ou **MIDI** (para gravacao de teclado/controlador).

Para alterar o tipo de faixa:

- Clique no botao **Options** na faixa, ou
- Clique no **icone do tipo de faixa** (entre "Options" e o campo de nome)

Isso abre o dialogo de Configuracao de Faixa, onde voce pode selecionar a fonte de entrada.

![Configuracao de faixa](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Controles de Faixa

Cada linha de faixa fornece os seguintes controles:

- **Caixa de selecao Enable**: Alterna se a faixa esta incluida na reproducao e mixagem.
  Desabilitar uma faixa tambem a desarma automaticamente, caso esteja armada.
- **Botao de radio Arm**: Seleciona esta faixa como alvo da gravacao. Apenas uma faixa
  pode estar armada por vez; armar uma nova faixa desarma automaticamente qualquer faixa
  anteriormente armada.
- **Campo de nome**: Campo de texto editavel para o nome da faixa. Este nome e usado como
  nome do arquivo de audio (com caracteres invalidos do sistema de arquivos substituidos
  por underscores).
- **Botao Options**: Abre o dialogo de Configuracao de Faixa.
- **Icone de tipo**: Mostra um icone de alto-falante para faixas de audio ou um icone de
  piano para faixas MIDI. Clicar nele abre o dialogo de Configuracao de Faixa.
- **Botao de remocao (x)**: Remove a faixa do projeto.

### Auto-Armamento

Quando um projeto tem exatamente uma faixa e essa faixa ainda nao foi gravada, ela e
automaticamente armada para gravacao. Isso se aplica tanto ao adicionar a primeira faixa
a um novo projeto quanto ao abrir um projeto existente que tenha uma unica faixa vazia.

### Visualizacao de Faixas

- **Faixas de audio** exibem uma visualizacao de forma de onda do audio gravado. Quando
  nenhum audio foi gravado, a area mostra "No audio recorded".
- **Faixas MIDI** exibem uma visualizacao de piano roll mostrando as notas gravadas em uma
  grade que abrange de A0 a C8. As notas sao coloridas por velocidade. Quando nenhum dado
  MIDI foi gravado, a area mostra "No MIDI data recorded".

## Gravacao

### Gravacao de Audio

1. Certifique-se de que o diretorio do projeto esta definido.
2. Arme a faixa alvo (marque o botao de radio "Arm").
3. Clique no botao **Record** (circulo vermelho).
4. Uma contagem regressiva de 3 segundos aparece na faixa ("Get ready... 3", "2", "1"),
   e entao a gravacao comeca.
5. Durante a gravacao, um medidor de nivel ao vivo e exibido na area de forma de onda da
   faixa, mostrando a amplitude atual como uma barra de gradiente (verde para amarelo para
   vermelho) com um rotulo "Recording".
6. Clique no botao **Stop** para encerrar a gravacao.

O audio gravado e salvo como um arquivo FLAC no diretorio do projeto, nomeado de acordo
com a faixa.

Durante a gravacao e reproducao, todos os controles interativos (botoes de faixa,
configuracoes, etc.) sao desabilitados para evitar alteracoes acidentais.

#### Gravacao Overdub

Ao gravar uma nova faixa enquanto outras faixas habilitadas ja contem dados de audio ou
MIDI, Musician's Canvas realiza gravacao overdub: as faixas existentes sao mixadas e
reproduzidas em tempo real enquanto a nova faixa esta sendo gravada. Isso permite que
voce ouca as partes gravadas anteriormente enquanto grava uma nova.

A mixagem das faixas existentes e preparada antes do inicio da captura, de modo que a
gravacao e a reproducao comecam aproximadamente no mesmo instante, mantendo todas as
faixas sincronizadas.

#### Backends de Gravacao

Musician's Canvas suporta dois backends de captura de audio:

- **PortAudio** (padrao quando disponivel): Fornece gravacao confiavel e de baixa latencia,
  sendo o backend recomendado.
- **Qt Multimedia**: Um backend alternativo que utiliza a captura de audio integrada do Qt.
  Usado quando PortAudio nao esta disponivel ou quando explicitamente selecionado nas
  Configuracoes do Projeto.

O backend de gravacao pode ser configurado por projeto em **Project > Project Settings >
Audio**.

#### Taxa de Amostragem e Tratamento de Dispositivos

Musician's Canvas grava na taxa de amostragem nativa do dispositivo de entrada de audio e
entao converte automaticamente para a taxa de amostragem configurada do projeto usando
reamostragem de alta qualidade. Isso significa que voce pode definir qualquer taxa de
amostragem do projeto (por exemplo, 44100 Hz ou 48000 Hz) independentemente da taxa
nativa do dispositivo. A conversao preserva o tom e a duracao com exatidao.

#### Deteccao de Dispositivos Mono

Alguns dispositivos de audio (por exemplo, microfones de webcam USB) sao fisicamente mono,
mas sao anunciados como estereo pelo sistema operacional. Musician's Canvas detecta isso
automaticamente e ajusta a contagem de canais de acordo. Se o projeto estiver configurado
para estereo, o sinal mono e duplicado para ambos os canais.

### Gravacao MIDI

1. Defina o tipo de faixa como **MIDI** pelo botao Options.
2. Certifique-se de que um dispositivo de entrada MIDI esta configurado em **Settings >
   Configuration > MIDI**.
3. Arme a faixa e clique em Record.
4. Toque notas no seu controlador MIDI.
5. Clique em Stop para encerrar a gravacao.

As notas MIDI sao exibidas em uma visualizacao de piano roll na faixa.

## Metronomo

Musician's Canvas inclui um metronomo integrado que pode ser usado durante a gravacao para
ajudar a manter o tempo. Clique no botao do metronomo na barra de botoes (abaixo da barra
de menus) para abrir o dialogo de configuracoes do metronomo:

![Configuracoes do metronomo](../screenshots/MetronomeSettings.png)

O dialogo oferece:

- **Enable metronome during recording**: Quando marcado, o metronomo toca um som de tique
  enquanto a gravacao esta ativa. O tique e reproduzido atraves do audio do sistema e **nao**
  e capturado na faixa gravada.
- **Beats per minute**: Uma entrada numerica para o andamento, em batidas por minuto (BPM).
  O intervalo e de 20 a 300 BPM.

Quando o metronomo esta habilitado, ele comeca a tiquetaquear assim que a gravacao realmente
comeca (apos o termino da contagem regressiva de 3 segundos) e para quando a gravacao termina.

## Reproducao

Clique no botao **Play** para mixar e reproduzir todas as faixas habilitadas. A dica do
botao muda para indicar se ele ira reproduzir ou gravar, dependendo de haver uma faixa
armada. Faixas desabilitadas (desmarcadas) sao excluidas da reproducao.

Durante a reproducao, faixas de audio sao decodificadas de seus arquivos FLAC e faixas
MIDI sao renderizadas em audio usando o sintetizador FluidSynth integrado. Todas as faixas
sao mixadas e reproduzidas pelo dispositivo de saida de audio do sistema.

Clique no botao **Stop** para encerrar a reproducao a qualquer momento.

## Mixagem para Arquivo

Use **Tools > Mix tracks to file** (Ctrl+M) para exportar todas as faixas habilitadas
para um unico arquivo de audio. Um dialogo permite que voce escolha o caminho de saida e
o formato:

- **Arquivo de saida**: Navegue para selecionar o caminho do arquivo de destino.
- **Formato**: Escolha entre FLAC (compressao sem perdas, arquivos menores) ou WAV
  (sem compressao).

A mixagem usa a taxa de amostragem configurada do projeto. Faixas MIDI sao renderizadas
usando o SoundFont configurado.

## Configuracoes

### Configuracoes Globais

Use **Settings > Configuration** (Ctrl+,) para definir padroes globais que se aplicam a
todos os projetos:

![Configuracoes gerais](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Aba General

- **Theme**: Escolha entre os temas Dark e Light.
- **Gravar log de depuracao de gravacao**: Quando habilitado, Musician's Canvas grava
  informacoes detalhadas de diagnostico de gravacao em um arquivo chamado
  `recording_debug.txt` no diretorio do projeto. Isso inclui o backend de captura, o nome
  do dispositivo, a taxa de amostragem, a contagem de canais e dados de temporizacao. Esta
  opcao esta desabilitada por padrao e e principalmente util para solucionar problemas de
  gravacao.

#### Aba Display

- **Cor do LED do visor numerico**: Escolha a cor usada no visor numerico LED de tempo
  mostrado na barra de ferramentas da janela principal. Os segmentos ativos dos digitos
  sao desenhados na cor escolhida, e os segmentos inativos sao desenhados como uma
  versao esmaecida da mesma cor. As cores disponiveis sao Light Red, Dark Red, Light
  Green, Dark Green, Light Blue, Dark Blue, Yellow, Orange, Light Cyan e Dark Cyan. O
  padrao e Light Green.

![Configuracoes de exibicao](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Aba Language

- **Idioma**: Escolha o idioma de exibicao do aplicativo. O padrao e "System Default",
  que utiliza a configuracao de idioma do sistema operacional. Os idiomas disponiveis sao
  English, Deutsch (German), Español (Spanish), Français (French), Japanese, Português
  (Brazilian Portuguese), Chinese (Traditional), Chinese (Simplified), Dansk (Danish),
  Ελληνικά (Greek), Gaeilge (Irish), Norsk (Norwegian), Polski (Polish), Русский
  (Russian), Suomi (Finnish), Svenska (Swedish), Cymraeg (Welsh) e Pirate. A interface e
  atualizada imediatamente ao alterar o idioma.

![Configuracoes de idioma](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### Aba MIDI

- **MIDI Output Device**: Selecione o sintetizador FluidSynth integrado ou um dispositivo
  MIDI externo. Use o botao **Refresh** para reescanear os dispositivos MIDI disponiveis.
- **SoundFont**: Navegue ate um arquivo SoundFont `.sf2` para sintese MIDI. No Linux, um
  SoundFont do sistema pode ser detectado automaticamente se o pacote `fluid-soundfont-gm`
  estiver instalado. No Windows e macOS, voce deve configurar o caminho do SoundFont
  manualmente.

![Configuracoes MIDI](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Aba Audio

- **Audio Input Device**: Selecione o microfone ou dispositivo de entrada de linha para
  gravacao.
- **Audio Output Device**: Selecione o alto-falante ou fone de ouvido para reproducao.

![Configuracoes de audio](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Configuracoes do Projeto

Use **Project > Project Settings** (Ctrl+P) para substituir os padroes globais apenas
para o projeto atual. Isso e util para projetos que precisam de uma taxa de amostragem,
SoundFont ou dispositivo de audio especificos. As configuracoes especificas do projeto
sao salvas dentro do arquivo `project.json`.

![Configuracoes MIDI do projeto](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Aba MIDI

- **MIDI Device**: Selecione um dispositivo MIDI para este projeto, ou deixe no padrao
  para usar a configuracao global.
- **SoundFont**: Selecione um arquivo SoundFont para este projeto.
- **Refresh**: Reescanear os dispositivos MIDI disponiveis.

#### Aba Audio

- **Audio Input Device**: Selecione o dispositivo de gravacao para este projeto.
- **Recording capture backend** (quando PortAudio esta disponivel):
  - **PortAudio (native input)**: Recomendado. Usa a mesma biblioteca de audio que o
    Audacity.
  - **Qt Multimedia**: Opcao alternativa que usa a captura de audio integrada do Qt.
- **PortAudio Input Device**: Ao usar o backend PortAudio, selecione o dispositivo de
  entrada PortAudio especifico.
- **Audio Output Device**: Selecione o dispositivo de reproducao para este projeto.

##### Configuracoes de Formato de Audio

- **Sample rate**: Escolha entre taxas padrao (8000 Hz a 192000 Hz). A taxa nativa do
  dispositivo e rotulada como "(native)". Taxas que requerem reamostragem sao rotuladas
  como "(resampled)". Voce pode selecionar qualquer taxa independentemente da capacidade
  do dispositivo; Musician's Canvas fara a reamostragem automaticamente conforme
  necessario.
- **Channels**: Mono ou Stereo. Se o dispositivo suportar apenas mono, a opcao Stereo
  e desabilitada.

![Configuracoes de audio do projeto](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Menus

### Menu File

| Item do Menu  | Atalho   | Descricao                                |
|---------------|----------|------------------------------------------|
| Save Project  | Ctrl+S   | Salvar o projeto atual no disco          |
| Open Project  | Ctrl+O   | Abrir um arquivo de projeto existente    |
| Close         | Ctrl+Q   | Sair do aplicativo                       |

### Menu Project

| Item do Menu                        | Atalho   | Descricao                                         |
|-------------------------------------|----------|----------------------------------------------------|
| Project Settings                    | Ctrl+P   | Configurar opcoes especificas do projeto           |
| Add Demo Data to Selected Track     |          | Adicionar notas MIDI de exemplo para demonstracao  |

### Menu Settings

| Item do Menu  | Atalho   | Descricao                                     |
|---------------|----------|------------------------------------------------|
| Configuration | Ctrl+,   | Abrir configuracoes globais do aplicativo      |

### Menu Tools

| Item do Menu          | Atalho   | Descricao                                     |
|-----------------------|----------|------------------------------------------------|
| Mix tracks to file    | Ctrl+M   | Exportar todas as faixas habilitadas para um arquivo |
| Virtual MIDI Keyboard |          | Iniciar o aplicativo de teclado complementar   |

## Atalhos de Teclado

| Atalho          | Acao                                |
|-----------------|-------------------------------------|
| Ctrl+S          | Salvar projeto                      |
| Ctrl+O          | Abrir projeto                       |
| Ctrl+M          | Mixar faixas para arquivo           |
| Ctrl+P          | Configuracoes do Projeto            |
| Ctrl+,          | Configuracoes / Configuration       |
| Ctrl+Q / Alt+F4 | Sair                               |

## Virtual MIDI Keyboard

Virtual MIDI Keyboard e um aplicativo complementar (`virtual_midi_keyboard`) que fornece
um teclado de piano na tela para envio de notas MIDI. Ele pode ser iniciado a partir do
menu **Tools > Virtual MIDI Keyboard** no aplicativo principal, ou executado
independentemente.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Recursos

- Clique nas teclas do piano na tela para tocar notas
- Use o teclado do computador como um teclado de piano (veja o mapeamento de teclas abaixo)
- Conecte-se a dispositivos de saida MIDI externos ou use o sintetizador FluidSynth
  integrado
- Conecte-se a um dispositivo de entrada MIDI para exibir notas recebidas no teclado
- Volume do sintetizador ajustavel (ganho mestre, 10%--200%)
- Selecao de SoundFont para o sintetizador integrado
- Selecao de instrumento/programa MIDI (General MIDI ou presets do SoundFont)
- Botao de controle de chorus/efeito
- Transposicao de oitava (-3 a +5)
- Controle de volume MIDI (CC#7, 0--127)

### Controles da Barra de Ferramentas

- **Volume**: Volume MIDI (CC#7), ajustavel de 0 a 127 pela caixa numerica.
- **Octave**: Mude a oitava do teclado usando os botoes **<** e **>** ou a caixa
  numerica. O intervalo e de -3 a +5.
- **Chorus/Effect**: Um botao rotativo e campo de texto (1--127) para definir o nivel de
  chorus/efeito (MIDI CC#93). Clique em **Apply** para enviar o valor.
- **Seletor de instrumento**: Escolha um instrumento MIDI. Ao usar o sintetizador
  FluidSynth integrado, isso mostra os presets do SoundFont (Bank:Program Name). Quando
  conectado a um dispositivo MIDI externo, lista os 128 instrumentos General MIDI.

### Tocando com o Teclado do Computador

O teclado do computador e mapeado para teclas de piano ao longo de duas oitavas:

**Oitava inferior (comecando na oitava atual):**

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

**Oitava superior (uma oitava acima):**

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
| I     | C (proxima oitava) |
| 9     | C#/Db |
| O     | D     |
| 0     | D#/Eb |
| P     | E     |

As teclas se iluminam visualmente quando pressionadas (teclas brancas ficam azul claro,
teclas pretas escurecem).

### Configuracao

Abra o dialogo de Configuracao (**Settings > Configuration**, Ctrl+,) para configurar
dispositivos MIDI e audio:

![Configuracao do Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard2.png)

#### Aba MIDI

- **MIDI Output Device**: Selecione um dispositivo externo ou deixe em branco para o
  sintetizador FluidSynth integrado.
- **MIDI Input Device**: Selecione um controlador para encaminhar notas para a exibicao
  do teclado.
- **Synthesizer Volume (Master Gain)**: Ajuste o nivel de saida do sintetizador integrado
  (10%--200%).
- **SoundFont**: Navegue ate um arquivo SoundFont `.sf2` para o sintetizador integrado.
- **Refresh**: Reescanear os dispositivos MIDI disponiveis.

#### Aba Audio

- **Audio Output Device**: Selecione o dispositivo de saida para o sintetizador integrado.

#### Aba Language

- **Idioma**: Escolha o idioma de exibicao. Os mesmos idiomas do Musician's Canvas estao
  disponiveis. A interface e atualizada imediatamente ao alterar o idioma.

### Atalhos do Virtual MIDI Keyboard

| Atalho   | Acao                        |
|----------|-----------------------------|
| Ctrl+,   | Dialogo de configuracao     |
| Ctrl+U   | Ajuda / Informacoes de uso  |
| Ctrl+Q   | Fechar                      |

## Solucao de Problemas

### Sem Saida de Audio

- Verifique se o dispositivo de saida de audio correto esta selecionado em Settings >
  Configuration > Audio.
- No Linux, verifique se o PipeWire ou PulseAudio esta em execucao e se a saida nao esta
  muda. Use `amixer` ou as configuracoes de som da sua area de trabalho para verificar os
  niveis de volume.
- No Windows, certifique-se de que um driver de audio ASIO esta instalado (por exemplo,
  [ASIO4ALL](https://asio4all.org/) ou um driver ASIO fornecido pelo fabricante para sua
  interface de audio). Musician's Canvas requer ASIO para audio de baixa latencia no
  Windows.

### Faixas MIDI Estao Silenciosas

- Certifique-se de que um SoundFont (arquivo `.sf2`) esta configurado em Settings >
  Configuration > MIDI.
- No Linux, um SoundFont do sistema pode ser detectado automaticamente se o pacote
  `fluid-soundfont-gm` estiver instalado.
- No Windows e macOS, voce deve configurar o caminho do SoundFont manualmente.

### A Gravacao Soa Distorcida ou Com Tom Errado

- Isso pode acontecer quando a taxa de amostragem nativa do dispositivo de entrada de
  audio difere da taxa configurada do projeto. Musician's Canvas lida com isso
  automaticamente via reamostragem, mas se os problemas persistirem, tente definir a taxa
  de amostragem do projeto para corresponder a taxa nativa do dispositivo.
- Microfones de webcam USB frequentemente possuem taxas nativas incomuns (por exemplo,
  32000 Hz). O aplicativo detecta essas taxas automaticamente.
- Se estiver usando o backend Qt Multimedia e experimentando problemas, tente mudar para
  o backend PortAudio em Project Settings > Audio.

### Virtual MIDI Keyboard Sem Audio

- No Linux com PipeWire, certifique-se de que o pacote `libpipewire-0.3-dev` esta
  instalado (necessario para integracao do PipeWire com o sintetizador FluidSynth).
- Verifique se um SoundFont esta carregado (veja a aba MIDI no dialogo de configuracao).
- Confirme que o dispositivo de saida de audio esta selecionado e que o volume do sistema
  nao esta mudo.

## Compilando a Partir do Codigo-Fonte

Consulte o [README](../README.md) para instrucoes completas de compilacao para Linux,
macOS e Windows, incluindo todas as dependencias necessarias.
