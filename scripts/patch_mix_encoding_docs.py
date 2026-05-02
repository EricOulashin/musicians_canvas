#!/usr/bin/env python3
"""README locales — mix encoding (MP3/Ogg) feature bullets and usage step 9."""
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

README_PATCHES: dict[str, tuple[str, str, str, str]] = {
    "es": (
        "- **Mezclar a un solo archivo de audio**: Exportar todas las pistas habilitadas a un solo archivo WAV o FLAC mezclado usando la biblioteca [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Mezclar a un solo archivo de audio**: Exportar todas las pistas habilitadas a un solo archivo mixado (WAV, FLAC, MP3, Ogg Vorbis o AIFF) mediante la biblioteca [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) — **Tools → Mix tracks to file**. La codificación usa las mismas clases que el proyecto relacionado (`MP3File`, `OggFile`, `AiffFile`) con **libsndfile**. Si el destino es **MP3** u **Ogg Vorbis**, el diálogo muestra la sección **Encoding** para modo de tasa de bits y calidad (p. ej. MP3 CBR/ABR/VBR y Ogg por calidad o tasa nominal aproximada); **WAV**, **FLAC** y **AIFF** no muestran controles de tasa de bits ahí.",
        '9. **Mezclar a archivo**: Use **Tools → Mix tracks to file** (Ctrl+M) para exportar a WAV o FLAC',
        "9. **Mezclar a archivo**: Use **Tools → Mix tracks to file** (Ctrl+M) para exportar a WAV, FLAC, MP3, Ogg Vorbis o AIFF. Para **MP3** y **Ogg**, configure la tasa de bits o la calidad en la sección **Encoding** del diálogo antes de mezclar.",
    ),
    "fr": (
        "- **Mixage vers un seul fichier audio** : Exporter toutes les pistes activées vers un seul fichier WAV ou FLAC mixé en utilisant la bibliothèque [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Mixage vers un seul fichier audio** : Exporter toutes les pistes activées vers un fichier mixé unique (WAV, FLAC, MP3, Ogg Vorbis ou AIFF) avec la bibliothèque [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). L’encodage reprend les mêmes classes que le projet associé (`MP3File`, `OggFile`, `AiffFile`) avec **libsndfile**. Pour une destination **MP3** ou **Ogg Vorbis**, la boîte de dialogue affiche une section **Encoding** pour le débit et la qualité (CBR/ABR/VBR pour le MP3, qualité ou débit nominal approximatif pour l’Ogg) ; **WAV**, **FLAC** et **AIFF** n’affichent pas ces réglages de débit.",
        "9. **Mixer vers un fichier** : Utilisez **Tools → Mix tracks to file** (Ctrl+M) pour exporter en WAV ou FLAC",
        "9. **Mixer vers un fichier** : Utilisez **Tools → Mix tracks to file** (Ctrl+M) pour exporter en WAV, FLAC, MP3, Ogg Vorbis ou AIFF. Pour **MP3** et **Ogg**, réglez le débit ou la qualité dans la section **Encoding** avant le mixage.",
    ),
    "ja": (
        "- **単一オーディオファイルへのミックス**：[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)ライブラリを使用して、有効なすべてのトラックを単一のミックスされたWAVまたはFLACファイルにエクスポート",
        "- **単一オーディオファイルへのミックス**：[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) ライブラリ（**Tools → Mix tracks to file**）により、有効なすべてのトラックを1つのミックスファイル（WAV、FLAC、MP3、Ogg Vorbis、AIFF）にエクスポート。エンコードは関連プロジェクトと同じクラス（`MP3File`、`OggFile`、`AiffFile`）と **libsndfile** を使用。出力が **MP3** または **Ogg Vorbis** のとき、ミックスダイアログに **Encoding** セクションが表示され、ビットレートモードや品質（MP3 の CBR/ABR/VBR、Ogg の品質ベースまたはおおよそのビットレート）を設定できます。**WAV**、**FLAC**、**AIFF** ではそのダイアログにビットレート設定はありません。",
        "9. **ファイルにミックス**：**Tools → Mix tracks to file**（Ctrl+M）を使用してWAVまたはFLACにエクスポート",
        "9. **ファイルにミックス**：**Tools → Mix tracks to file**（Ctrl+M）を使用して WAV、FLAC、MP3、Ogg Vorbis、AIFF にエクスポート。**MP3** と **Ogg** では、ミックス前にダイアログの **Encoding** でビットレートまたは品質を設定します。",
    ),
    "zh-CN": (
        "- **混音为单一音频文件**：使用 [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) 库将所有启用的轨道导出为单一混音的 WAV 或 FLAC 文件",
        "- **混音为单一音频文件**：使用 [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) 库（**Tools → Mix tracks to file**）将所有启用的轨道导出为单个混音文件（WAV、FLAC、MP3、Ogg Vorbis、AIFF）。编码使用同源项目的相同类（`MP3File`、`OggFile`、`AiffFile`）以及 **libsndfile**。若目标格式为 **MP3** 或 **Ogg Vorbis**，混音对话框会显示 **Encoding** 分区，用于设置比特率模式与音质（例如 MP3 的 CBR/ABR/VBR，以及 Ogg 的音质型 VBR 或近似标称比特率）；**WAV**、**FLAC**、**AIFF** 不在该对话框中提供比特率选项。",
        "9. **混音为文件**：使用 **Tools → Mix tracks to file**（Ctrl+M）导出为 WAV 或 FLAC",
        "9. **混音为文件**：使用 **Tools → Mix tracks to file**（Ctrl+M）导出为 WAV、FLAC、MP3、Ogg Vorbis、AIFF。**MP3** 与 **Ogg** 请在混音前于对话框的 **Encoding** 分区设置比特率或音质。",
    ),
    "zh-TW": (
        "- **混音為單一音訊檔案**：使用 [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) 函式庫將所有啟用的軌道匯出為單一混音的 WAV 或 FLAC 檔案",
        "- **混音為單一音訊檔案**：使用 [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) 函式庫（**Tools → Mix tracks to file**）將所有啟用的軌道匯出為單一混音檔（WAV、FLAC、MP3、Ogg Vorbis、AIFF）。編碼沿用同源專案的類別（`MP3File`、`OggFile`、`AiffFile`）與 **libsndfile**。若目的地為 **MP3** 或 **Ogg Vorbis**，混音對話框會顯示 **Encoding** 區塊以設定位元率模式與品質（例如 MP3 的 CBR/ABR/VBR，以及 Ogg 的品質型 VBR 或近似標稱位元率）；**WAV**、**FLAC**、**AIFF** 在此對話框不提供位元率選項。",
        "9. **混音為檔案**：使用 **Tools → Mix tracks to file**（Ctrl+M）匯出為 WAV 或 FLAC",
        "9. **混音為檔案**：使用 **Tools → Mix tracks to file**（Ctrl+M）匯出為 WAV、FLAC、MP3、Ogg Vorbis、AIFF。**MP3** 與 **Ogg** 請在混音前於對話框的 **Encoding** 區設定位元率或品質。",
    ),
    "ru": (
        "- **Сведение в один аудиофайл**: Экспорт всех включённых дорожек в один смикшированный WAV или FLAC файл с использованием библиотеки [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Сведение в один аудиофайл**: Экспорт всех включённых дорожек в один смикшированный файл (WAV, FLAC, MP3, Ogg Vorbis или AIFF) с помощью библиотеки [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Кодирование использует те же классы, что и родственный проект (`MP3File`, `OggFile`, `AiffFile`), и **libsndfile**. Для **MP3** и **Ogg Vorbis** в диалоге сведения показывается раздел **Encoding** для режима битрейта и качества (CBR/ABR/VBR для MP3, качество или приблизительный номинальный битрейт для Ogg); для **WAV**, **FLAC** и **AIFF** настройки битрейта там недоступны.",
        "9. **Сведение в файл**: Используйте **Tools → Mix tracks to file** (Ctrl+M) для экспорта в WAV или FLAC",
        "9. **Сведение в файл**: Используйте **Tools → Mix tracks to file** (Ctrl+M) для экспорта в WAV, FLAC, MP3, Ogg Vorbis или AIFF. Для **MP3** и **Ogg** задайте битрейт или качество в разделе **Encoding** перед сведением.",
    ),
    "sv": (
        "- **Mixa till en enda ljudfil**: Exportera alla aktiverade spår till en enda mixad WAV- eller FLAC-fil med hjälp av biblioteket [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Mixa till en enda ljudfil**: Exportera alla aktiverade spår till en enda mixad fil (WAV, FLAC, MP3, Ogg Vorbis eller AIFF) med biblioteket [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Kodning använder samma klasser som syskonprojektet (`MP3File`, `OggFile`, `AiffFile`) tillsammans med **libsndfile**. Vid destination **MP3** eller **Ogg Vorbis** visas avsnittet **Encoding** för bithastighetsläge och kvalitet (MP3 CBR/ABR/VBR och Ogg kvalitet eller ungefärlig nominell bithastighet); **WAV**, **FLAC** och **AIFF** har inga sådana reglage.",
        "9. **Mixa till fil**: Använd **Tools → Mix tracks to file** (Ctrl+M) för att exportera till WAV eller FLAC",
        "9. **Mixa till fil**: Använd **Tools → Mix tracks to file** (Ctrl+M) för att exportera till WAV, FLAC, MP3, Ogg Vorbis eller AIFF. För **MP3** och **Ogg**, ställ in bithastighet eller kvalitet under **Encoding** innan du mixar.",
    ),
    "fi": (
        "- **Miksaus yhdeksi äänitiedostoksi**: Vie kaikki käytössä olevat raidat yhdeksi miksatuksi WAV- tai FLAC-tiedostoksi [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-kirjastolla",
        "- **Miksaus yhdeksi äänitiedostoksi**: Vie kaikki käytössä olevat raidat yhdeksi miksatuksi tiedostoksi (WAV, FLAC, MP3, Ogg Vorbis tai AIFF) [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-kirjastolla (**Tools → Mix tracks to file**). Koodaus käyttää samaa luokkaperhettä (`MP3File`, `OggFile`, `AiffFile`) ja **libsndfile**-kirjastoa. Kun kohde on **MP3** tai **Ogg Vorbis**, miksausdialogissa näkyy **Encoding**-osio bittinopeuden ja laadun valintaan (MP3 CBR/ABR/VBR ja Ogg laatu tai likimääräinen nimellinen bittinopeus); **WAV**-, **FLAC**- ja **AIFF**-vienti ei tarjoa näitä valintoja.",
        "9. **Miksaa tiedostoon**: Käytä **Tools → Mix tracks to file** (Ctrl+M) viedäksesi WAV- tai FLAC-muotoon",
        "9. **Miksaa tiedostoon**: Käytä **Tools → Mix tracks to file** (Ctrl+M) viedäksesi WAV-, FLAC-, MP3-, Ogg Vorbis- tai AIFF-muotoon. **MP3**- ja **Ogg** -kohteille valitse bittinopeus tai laatu **Encoding**-osiosta ennen miksausta.",
    ),
    "da": (
        "- **Miks til enkelt lydfil**: Eksporter alle aktiverede spor til en enkelt mikset WAV- eller FLAC-fil ved hjælp af [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-biblioteket",
        "- **Miks til enkelt lydfil**: Eksporter alle aktiverede spor til én mikset fil (WAV, FLAC, MP3, Ogg Vorbis eller AIFF) med [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Kodning bruger de samme klasser som søsterprojektet (`MP3File`, `OggFile`, `AiffFile`) og **libsndfile**. Ved destination **MP3** eller **Ogg Vorbis** vises **Encoding** til bitrate og kvalitet (MP3 CBR/ABR/VBR og Ogg kvalitet eller ca. nominel bitrate); **WAV**, **FLAC** og **AIFF** har ikke disse valg.",
        "9. **Miks til fil**: Brug **Tools → Mix tracks to file** (Ctrl+M) til at eksportere til WAV eller FLAC",
        "9. **Miks til fil**: Brug **Tools → Mix tracks to file** (Ctrl+M) til at eksportere til WAV, FLAC, MP3, Ogg Vorbis eller AIFF. For **MP3** og **Ogg** skal du indstille bitrate eller kvalitet under **Encoding** før miks.",
    ),
    "nb": (
        "- **Miks til enkelt lydfil**: Eksporter alle aktiverte spor til en enkelt mikset WAV- eller FLAC-fil ved hjelp av [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-biblioteket",
        "- **Miks til enkelt lydfil**: Eksporter alle aktiverte spor til én mikset fil (WAV, FLAC, MP3, Ogg Vorbis eller AIFF) med [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Kodning bruker de samme klassene som søsterprosjektet (`MP3File`, `OggFile`, `AiffFile`) og **libsndfile**. For **MP3** eller **Ogg Vorbis** vises **Encoding** for bitrate og kvalitet (MP3 CBR/ABR/VBR og Ogg kvalitet eller omtrentlig nominell bitrate); **WAV**, **FLAC** og **AIFF** har ikke disse innstillingene.",
        "9. **Miks til fil**: Bruk **Tools → Mix tracks to file** (Ctrl+M) for å eksportere til WAV eller FLAC",
        "9. **Miks til fil**: Bruk **Tools → Mix tracks to file** (Ctrl+M) for å eksportere til WAV, FLAC, MP3, Ogg Vorbis eller AIFF. For **MP3** og **Ogg**, sett bitrate eller kvalitet i **Encoding** før du mikser.",
    ),
    "pl": (
        "- **Miksowanie do pojedynczego pliku audio**: Eksport wszystkich włączonych ścieżek do pojedynczego zmiksowanego pliku WAV lub FLAC przy użyciu biblioteki [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Miksowanie do pojedynczego pliku audio**: Eksport wszystkich włączonych ścieżek do pojedynczego zmiksowanego pliku (WAV, FLAC, MP3, Ogg Vorbis lub AIFF) przy użyciu biblioteki [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Kodowanie wykorzystuje te same klasy co projekt pokrewny (`MP3File`, `OggFile`, `AiffFile`) oraz **libsndfile**. Gdy celem jest **MP3** lub **Ogg Vorbis**, dialog miksu pokazuje sekcję **Encoding** do wyboru trybu bitrate i jakości (MP3 CBR/ABR/VBR oraz Ogg wg jakości lub przybliżonego nominalnego bitrate); **WAV**, **FLAC** i **AIFF** nie oferują tam ustawień bitrate.",
        "9. **Miksuj do pliku**: Użyj **Tools → Mix tracks to file** (Ctrl+M), aby wyeksportować do WAV lub FLAC",
        "9. **Miksuj do pliku**: Użyj **Tools → Mix tracks to file** (Ctrl+M), aby wyeksportować do WAV, FLAC, MP3, Ogg Vorbis lub AIFF. Dla **MP3** i **Ogg** ustaw bitrate lub jakość w sekcji **Encoding** przed miksem.",
    ),
    "el": (
        "- **Μίξη σε ενιαίο αρχείο ήχου**: Εξαγωγή όλων των ενεργοποιημένων καναλιών σε ένα ενιαίο μεικτό αρχείο WAV ή FLAC χρησιμοποιώντας τη βιβλιοθήκη [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Μίξη σε ενιαίο αρχείο ήχου**: Εξαγωγή όλων των ενεργοποιημένων καναλιών σε ένα ενιαίο μεικτό αρχείο (WAV, FLAC, MP3, Ogg Vorbis ή AIFF) με τη βιβλιοθήκη [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Η κωδικοποίηση χρησιμοποιεί τις ίδιες κλάσεις με το συγγενικό έργο (`MP3File`, `OggFile`, `AiffFile`) και το **libsndfile**. Για προορισμό **MP3** ή **Ogg Vorbis**, ο διάλογος μίξης εμφανίζει ενότητα **Encoding** για ρυθμό και ποιότητα (MP3 CBR/ABR/VBR και Ogg ποιότητα ή κατά προσέγγιση nominal bitrate)· τα **WAV**, **FLAC** και **AIFF** δεν προσφέρουν εκεί ρυθμό μετάδοσης.",
        "9. **Μίξη σε αρχείο**: Χρησιμοποιήστε **Tools → Mix tracks to file** (Ctrl+M) για εξαγωγή σε WAV ή FLAC",
        "9. **Μίξη σε αρχείο**: Χρησιμοποιήστε **Tools → Mix tracks to file** (Ctrl+M) για εξαγωγή σε WAV, FLAC, MP3, Ogg Vorbis ή AIFF. Για **MP3** και **Ogg**, ορίστε bitrate ή ποιότητα στην ενότητα **Encoding** πριν από τη μίξη.",
    ),
    "ga": (
        "- **Measc go comhad fuaime singil**: Easpórtáil na rianta cumasaithe go léir go comhad singil measctha WAV nó FLAC ag úsáid leabharlann [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Measc go comhad fuaime singil**: Easpórtáil na rianta cumasaithe go léir go comhad measctha amháin (WAV, FLAC, MP3, Ogg Vorbis nó AIFF) ag baint úsáide as leabharlann [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Úsáideann an ionchódú na ranganna céanna leis an tionscadal gaolta (`MP3File`, `OggFile`, `AiffFile`) le **libsndfile**. Nuair is **MP3** nó **Ogg Vorbis** an spriocean, taispeántar rannóg **Encoding** sa dialóg le hidrbande agus caighdeán (CBR/ABR/VBR MP3 agus Vorbis de réir caighdeáin nó bitrate ainmnithe gar-mheasta); ní thairgtear roghanna bitrate ann do **WAV**, **FLAC** ná **AIFF**.",
        "9. **Measc go comhad**: Úsáid **Tools → Mix tracks to file** (Ctrl+M) chun easpórtáil go WAV nó FLAC",
        "9. **Measc go comhad**: Úsáid **Tools → Mix tracks to file** (Ctrl+M) chun easpórtáil go WAV, FLAC, MP3, Ogg Vorbis nó AIFF. Do **MP3** agus **Ogg**, socraigh bitrate nó caighdeán sa rannóg **Encoding** roimh an meascadh.",
    ),
    "cy": (
        "- **Cymysgu i un ffeil sain**: Allforio'r holl draciau a alluogwyd i un ffeil WAV neu FLAC wedi'i chymysgu gan ddefnyddio llyfrgell [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Cymysgu i un ffeil sain**: Allforio'r holl draciau a alluogwyd i un ffeil gymysg (WAV, FLAC, MP3, Ogg Vorbis neu AIFF) gan ddefnyddio llyfrgell [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Mae'r amgodio yn defnyddio'r un dosbarthiadau â'r prosiect brawd (`MP3File`, `OggFile`, `AiffFile`) gyda **libsndfile**. Pan fo'r cyrchfan yn **MP3** neu **Ogg Vorbis**, mae'r dialog yn dangos adran **Encoding** ar gyfer modd diddeb a manylder (CBR/ABR/VBR MP3 ac Ogg yn ôl ansawdd neu ddiddeb enwol bras); nid oes rheolaethau diddeb yn y dialog ar gyfer **WAV**, **FLAC** na **AIFF**.",
        "9. **Cymysgu i ffeil**: Defnyddiwch **Tools → Mix tracks to file** (Ctrl+M) i allforio i WAV neu FLAC",
        "9. **Cymysgu i ffeil**: Defnyddiwch **Tools → Mix tracks to file** (Ctrl+M) i allforio i WAV, FLAC, MP3, Ogg Vorbis neu AIFF. Ar gyfer **MP3** ac **Ogg**, dewiswch diddeb neu ansawdd yn yr adran **Encoding** cyn cymysgu.",
    ),
    "pt-BR": (
        "- **Mixar para um único arquivo de áudio**: Exportar todas as faixas habilitadas para um único arquivo WAV ou FLAC mixado usando a biblioteca [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)",
        "- **Mixar para um único arquivo de áudio**: Exportar todas as faixas habilitadas para um único arquivo mixado (WAV, FLAC, MP3, Ogg Vorbis ou AIFF) usando a biblioteca [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). A codificação usa as mesmas classes do projeto relacionado (`MP3File`, `OggFile`, `AiffFile`) com **libsndfile**. Quando o destino é **MP3** ou **Ogg Vorbis**, o diálogo exibe a seção **Encoding** para modo de taxa de bits e qualidade (CBR/ABR/VBR no MP3 e Ogg por qualidade ou taxa nominal aproximada); **WAV**, **FLAC** e **AIFF** não oferecem essas opções.",
        "9. **Mixar para arquivo**: Use **Tools → Mix tracks to file** (Ctrl+M) para exportar em WAV ou FLAC",
        "9. **Mixar para arquivo**: Use **Tools → Mix tracks to file** (Ctrl+M) para exportar em WAV, FLAC, MP3, Ogg Vorbis ou AIFF. Para **MP3** e **Ogg**, defina taxa de bits ou qualidade na seção **Encoding** antes de mixar.",
    ),
}


def patch_readme(lang: str, old_f: str, new_f: str, old_u: str, new_u: str) -> None:
    path = ROOT / f"README_{lang}.md"
    text = path.read_text(encoding="utf-8")
    if old_f not in text:
        raise SystemExit(f"{path}: feature line not found")
    text = text.replace(old_f, new_f, 1)
    if old_u not in text:
        raise SystemExit(f"{path}: usage line not found")
    text = text.replace(old_u, new_u, 1)
    path.write_text(text, encoding="utf-8")


def main() -> None:
    for lang, tpl in README_PATCHES.items():
        patch_readme(lang, *tpl)
        print("README", lang)
    print("done")


if __name__ == "__main__":
    main()
