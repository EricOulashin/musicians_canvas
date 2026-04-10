#!/usr/bin/env python3
"""Generate scripts/effect_i18n.tsv (run from repo root: python3 scripts/build_effect_tsv.py)."""

from __future__ import annotations

import csv
from pathlib import Path

LANGS = [
    "de_DE",
    "es_ES",
    "fr_FR",
    "ja_JP",
    "pt_BR",
    "zh_CN",
    "zh_TW",
    "da_DK",
    "el_GR",
    "ga_IE",
    "nb_NO",
    "pl_PL",
    "ru_RU",
    "fi_FI",
    "sv_SE",
    "cy_GB",
    "en_PIRATE",
]

# fmt: off
# Each row: context, source, then one string per language in LANGS order (must match Qt .ts language attrs).
RAW: list[tuple[str, str, tuple[str, ...]]] = [
    ("ChorusWidget", "Chorus", (
        "Chor", "Coro", "Chorus", "コーラス", "Coro", "合唱", "合唱", "Kor", "Χορός", "Cór",
        "Kor", "Chór", "Хор", "Kuoro", "Kör", "Côr", "Chorus, ye scurvy dog!",
    )),
    ("ChorusWidget", "Effect level: %1%", (
        "Effektpegel: %1%", "Nivel del efecto: %1%", "Niveau d’effet : %1%", "エフェクトレベル: %1%", "Nível do efeito: %1%", "效果电平：%1%", "效果位準：%1%", "Effektniveau: %1%", "Επίπεδο εφέ: %1%", "Leibhéal éifeachta: %1%", "Effektnivå: %1%", "Poziom efektu: %1%", "Уровень эффекта: %1%", "Tehosteen taso: %1%", "Effektnivå: %1%", "Lefel effaith: %1%", "Effect level: %1% (aarrr!)",
    )),
    ("ChorusWidget", "Rate: %1 Hz", (
        "Rate: %1 Hz", "Velocidad: %1 Hz", "Fréquence : %1 Hz", "レート: %1 Hz", "Taxa: %1 Hz", "速率：%1 Hz", "速率：%1 Hz", "Hastighed: %1 Hz", "Ρυθμός: %1 Hz", "Ráta: %1 Hz", "Hastighet: %1 Hz", "Tempo: %1 Hz", "Скорость: %1 Гц", "Taajuus: %1 Hz", "Hastighet: %1 Hz", "Cyflymder: %1 Hz", "Rate: %1 Hz (ship’s wheel)",
    )),
    ("ChorusWidget", "Depth: %1 ms", (
        "Tiefe: %1 ms", "Profundidad: %1 ms", "Profondeur : %1 ms", "深さ: %1 ms", "Profundidade: %1 ms", "深度：%1 ms", "深度：%1 ms", "Dybde: %1 ms", "Βάθος: %1 ms", "Doimneas: %1 ms", "Dybde: %1 ms", "Głębokość: %1 ms", "Глубина: %1 мс", "Syvyys: %1 ms", "Djup: %1 ms", "Dwfn: %1 ms", "Depth: %1 ms",
    )),
    ("ChorusWidget", "EQ low: %1 dB", (
        "EQ tief: %1 dB", "EQ graves: %1 dB", "EQ graves : %1 dB", "EQ低域: %1 dB", "EQ grave: %1 dB", "低频均衡：%1 dB", "低頻等化：%1 dB", "EQ lav: %1 dB", "EQ χαμηλά: %1 dB", "EQ íseal: %1 dB", "EQ lav: %1 dB", "EQ niskie: %1 dB", "НЧ EQ: %1 дБ", "EQ matala: %1 dB", "EQ låg: %1 dB", "EQ isel: %1 dB", "EQ low: %1 dB, matey",
    )),
    ("ChorusWidget", "EQ high: %1 dB", (
        "EQ hoch: %1 dB", "EQ agudos: %1 dB", "EQ aigus : %1 dB", "EQ高域: %1 dB", "EQ agudo: %1 dB", "高频均衡：%1 dB", "高頻等化：%1 dB", "EQ høj: %1 dB", "EQ ψηλά: %1 dB", "EQ ard: %1 dB", "EQ høy: %1 dB", "EQ wysokie: %1 dB", "ВЧ EQ: %1 дБ", "EQ korkea: %1 dP", "EQ hög: %1 dB", "EQ uchel: %1 dB", "EQ high: %1 dB",
    )),
    ("ChorusWidget", "Wet chorus level", (
        "Nass-/Chorus-Pegel", "Nivel del chorus húmedo", "Niveau chorus humide", "ウェット・コーラスレベル", "Nível de chorus molhado", "湿声合唱力度", "濕訊合唱位準", "Vådt kor-niveau", "Επίπεδο υγρού χορού", "Leibhéal córais fliuch", "Våt kor-nivå", "Poziom mokrego chorus", "Уровень wet chorus", "Kostean kuoron taso", "Våt körnivå", "Lefel côr gwlyb", "Wet chorus grog, arr!",
    )),
    ("ChorusWidget", "LFO rate (Hz ×10)", (
        "LFO-Rate (Hz ×10)", "Velocidad LFO (Hz ×10)", "Vitesse LFO (Hz ×10)", "LFOレート (Hz ×10)", "Taxa LFO (Hz ×10)", "LFO 速率（Hz ×10）", "LFO 速率（Hz ×10）", "LFO-hastighed (Hz ×10)", "Ρυθμός LFO (Hz ×10)", "Ráta LFO (Hz ×10)", "LFO-hastighet (Hz ×10)", "Tempo LFO (Hz ×10)", "Скорость LFO (Гц ×10)", "LFO-taajuus (Hz ×10)", "LFO-hastighet (Hz ×10)", "Cyflymder LFO (Hz ×10)", "LFO rate (Hz ×10)",
    )),
    ("ChorusWidget", "Modulation depth (ms ×10)", (
        "Modulationstiefe (ms ×10)", "Profundidad de modulación (ms ×10)", "Profondeur de modulation (ms ×10)", "変調の深さ (ms ×10)", "Profundidade de modulação (ms ×10)", "调制深度（ms ×10）", "調制深度（ms ×10）", "Modulationsdybde (ms ×10)", "Βάθος διαμόρφωσης (ms ×10)", "Doimneas modhnaithe (ms ×10)", "Modulasjonsdybde (ms ×10)", "Głębokość modulacji (ms ×10)", "Глубина модуляции (мс ×10)", "Modulaatiosyvyys (ms ×10)", "Modulationsdjup (ms ×10)", "Dwfn cyfaddasu (ms ×10)", "Mod depth (ms ×10)",
    )),
    ("ChorusWidget", "Low-frequency emphasis on wet signal (dB ×10)", (
        "Betont tiefe Frequenzen im Nass-Signal (dB ×10)", "Énfasis de graves en señal húmeda (dB ×10)", "Accent graves sur signal humide (dB ×10)", "湿声の低音を強調 (dB ×10)", "Ênfase de graves no sinal molhado (dB ×10)", "湿声低频强调（dB ×10）", "濕訊低頻強調（dB ×10）", "Lavtoneffekt på vådt signal (dB ×10)", "Έμφαση χαμηλών σε υγρό σήμα (dB ×10)", "Béim ar mhion-thoinneog ar chomhartha fliuch (dB ×10)", "Lavtoner på vått signal (dB ×10)", "Akcent niskich na sygnale mokrym (dB ×10)", "НЧ на мокром сигнале (дБ ×10)", "Matalat korostus märkään signaaliin (dB ×10)", "Lågtonbetoning på våt signal (dB ×10)", "Pwyslais isel ar arwydd gwlyb (dB ×10)", "Low rumble on wet signal (dB ×10)",
    )),
    ("ChorusWidget", "High-frequency emphasis on wet signal (dB ×10)", (
        "Betont hohe Frequenzen im Nass-Signal (dB ×10)", "Énfasis de agudos en señal húmeda (dB ×10)", "Accent aigus sur signal humide (dB ×10)", "湿声の高音を強調 (dB ×10)", "Ênfase de agudos no sinal molhado (dB ×10)", "湿声高频强调（dB ×10）", "濕訊高頻強調（dB ×10）", "Høje toner på vådt signal (dB ×10)", "Έμφαση ψηλών σε υγρό σήμα (dB ×10)", "Béim ar ardtoinneog ar chomhartha fliuch (dB ×10)", "Høye toner på vått signal (dB ×10)", "Akcent wysokich na sygnale mokrym (dB ×10)", "ВЧ на мокром сигнале (дБ ×10)", "Korkeat korostus märkään signaaliin (dB ×10)", "Diskantbetoning på våt signal (dB ×10)", "Pwyslais uchel ar arwydd gwlyb (dB ×10)", "Bright spray on wet signal (dB ×10)",
    )),
    ("EffectWidget", "Drag to reorder effects", (
        "Ziehen, um Effekte neu anzuordnen", "Arrastre para reordenar efectos", "Glisser pour réorganiser les effets", "ドラッグしてエフェクトを並べ替え", "Arraste para reordenar efeitos", "拖動以重新排序效果", "拖曳以重新排序效果", "Træk for at omarrangere effekter", "Σύρετε για αναδιάταξη εφέ", "Romhnaigh chun éifeachtaí a athordu", "Dra for å stokke om effekter", "Przeciągnij, by zmienić kolejność efektów", "Перетащите для изменения порядка", "Järjestä efektit uudelleen vetämällä", "Dra för att ordna om effekter", "Llusgo i ad-drefnu effeithiau", "Drag to shuffle the effects, matey",
    )),
    ("EffectWidget", "Remove effect", (
        "Effekt entfernen", "Quitar efecto", "Retirer l’effet", "エフェクトを削除", "Remover efeito", "移除效果", "移除效果", "Fjern effekt", "Αφαίρεση εφέ", "Bain éifeacht", "Fjern effekt", "Usuń efekt", "Удалить эффект", "Poista tehoste", "Ta bort effekt", "Tynnu effaith", "Walk the effect off the plank",
    )),
    ("EffectsDialog", "Track effects", (
        "Spureffekte", "Efectos de pista", "Effets de piste", "トラックエフェクト", "Efeitos da faixa", "轨道效果", "軌道效果", "Spor-effekter", "Εφέ κομματιού", "Éifeachtaí rian", "Spor-effekter", "Efekty ścieżki", "Эффекты дорожки", "Raidan tehosteet", "Spår-effekter", "Effeithiau trac", "Effects fer yer track",
    )),
    ("EffectsDialog", "Add effect…", (
        "Effekt hinzufügen…", "Añadir efecto…", "Ajouter un effet…", "エフェクトを追加…", "Adicionar efeito…", "添加效果…", "加入效果…", "Tilføj effekt…", "Προσθήκη εφέ…", "Cuir éifeacht leis…", "Legg til effekt…", "Dodaj efekt…", "Добавить эффект…", "Lisää tehoste…", "Lägg till effekt…", "Ychwanegu effaith…", "Add effect… (yo-ho-ho)",
    )),
    ("EffectsDialog", "Add effect", (
        "Effekt hinzufügen", "Añadir efecto", "Ajouter un effet", "エフェクトを追加", "Adicionar efeito", "添加效果", "加入效果", "Tilføj effekt", "Προσθήκη εφέ", "Cuir éifeacht leis", "Legg til effekt", "Dodaj efekt", "Добавить эффект", "Lisää tehoste", "Lägg till effekt", "Ychwanegu effaith", "Add effect",
    )),
    ("EffectsDialog", "Choose an effect:", (
        "Effekt wählen:", "Elija un efecto:", "Choisissez un effet :", "エフェクトを選択:", "Escolha um efeito:", "选择效果:", "選擇效果:", "Vælg en effekt:", "Επιλέξτε εφέ:", "Roghnaigh éifeacht:", "Velg en effekt:", "Wybierz efekt:", "Выберите эффект:", "Valitse tehoste:", "Välj en effekt:", "Dewiswch effaith:", "Pick yer poison:",
    )),
    ("EffectsDialog", "Reverb", (
        "Hall", "Reverb", "Réverb", "リバーブ", "Reverb", "混响", "殘響", "Rumklang", "Ηχώ", "Athfhuaimniú", "Romklang", "Pogłos", "Реверб", "Kaiku", "Klangeko", "Adlais", "Reverb from Davy Jones’ locker",
    )),
    ("EffectsDialog", "Chorus", (
        "Chorus", "Coro", "Chorus", "コーラス", "Coro", "合唱", "合唱", "Kor", "Χορός", "Cór", "Kor", "Chór", "Хорус", "Kuoro", "Kör", "Côr", "Chorus o’ the waves",
    )),
    ("EffectsDialog", "Effects", (
        "Effekte", "Efectos", "Effets", "エフェクト", "Efeitos", "效果", "效果", "Effekter", "Εφέ", "Éifeachtaí", "Effekter", "Efekty", "Эффекты", "Tehosteet", "Effekter", "Effeithiau", "Effects",
    )),
    ("EffectsDialog", "Unknown effect type.", (
        "Unbekannter Effekttyp.", "Tipo de efecto desconocido.", "Type d’effet inconnu.", "不明なエフェクト種類。", "Tipo de efeito desconhecido.", "未知的效果类型。", "未知的效果類型。", "Ukendt effekttype.", "Άγνωστος τύπος εφέ.", "Cineál éifeachta anaithnid.", "Ukjent effekttype.", "Nieznany typ efektu.", "Неизвестный тип эффекта.", "Tuntematon tehostetyyppi.", "Okänd effekttyp.", "Math effaith anhysbys.", "That effect be not on the map.",
    )),
    ("ReverbWidget", "Reverb", (
        "Hall", "Reverb", "Réverb", "リバーブ", "Reverb", "混响", "殘響", "Rumklang", "Ηχώ", "Athfhuaimniú", "Romklang", "Pogłos", "Реверб", "Kaiku", "Klangeko", "Adlais", "Reverb",
    )),
    ("ReverbWidget", "Pre-delay: %1 ms", (
        "Vorhalt: %1 ms", "Pre-delay: %1 ms", "Pré-délai : %1 ms", "プリディレイ: %1 ms", "Pré-atraso: %1 ms", "预延迟：%1 ms", "前置延遲：%1 ms", "Forforstyrrelse: %1 ms", "Προκαθυστέρηση: %1 ms", "Réamh-mhoill: %1 ms", "Forsinkelse: %1 ms", "Pre-opóźnienie: %1 ms", "Предзадержка: %1 мс", "Esiviive: %1 ms", "Fördröjning: %1 ms", "Rhag-ddilyniant: %1 ms", "Pre-delay: %1 ms",
    )),
    ("ReverbWidget", "Decay: %1 s", (
        "Abklingzeit: %1 s", "Decaimiento: %1 s", "Déclin : %1 s", "減衰: %1 s", "Decay: %1 s", "衰减：%1 秒", "衰減：%1 秒", "Udpæling: %1 s", "Εξασθένηση: %1 s", "Laghú: %1 s", "Demping: %1 s", "Zanikanie: %1 s", "Затухание: %1 с", "Vaimennus: %1 s", "Urslockning: %1 s", "Dirwyn: %1 s", "Decay: %1 s",
    )),
    ("ReverbWidget", "Mix (wet): %1%", (
        "Mix (nass): %1%", "Mezcla (húmedo): %1%", "Mix (humide) : %1%", "ミックス（ウェット）: %1%", "Mistura (molhado): %1%", "混音（湿声）：%1%", "混音（濕訊）：%1%", "Mix (vådt): %1%", "Μείγμα (υγρό): %1%", "Meascán (fliuch): %1%", "Miks (våt): %1%", "Miks (mokro): %1%", "Микс (wet): %1%", "Miks (märkä): %1%", "Mix (våt): %1%", "Cymysg (gwlyb): %1%", "Mix (wet): %1%",
    )),
    ("ReverbWidget", "Tone: %1", (
        "Klangfarbe: %1", "Tono: %1", "Tonalité : %1", "トーン: %1", "Tom: %1", "音调：%1", "音色：%1", "Klang: %1", "Τόνος: %1", "Stíl: %1", "Klang: %1", "Barwa: %1", "Тон: %1", "Sävy: %1", "Klang: %1", "Tôn: %1", "Tone: %1",
    )),
    ("ReverbWidget", "dark", (
        "dunkel", "oscuro", "sombre", "ダーク", "escuro", "暗", "暗", "mørk", "σκοτεινό", "dorcha", "mørk", "ciemny", "тёмный", "tumma", "mörk", "tywyll", "dark as the hold",
    )),
    ("ReverbWidget", "bright", (
        "hell", "brillante", "clair", "ブライト", "claro", "明亮", "明亮", "lys", "φωτεινό", "geal", "lys", "jasny", "яркий", "kirkas", "ljus", "disglair", "bright as a doubloon",
    )),
    ("ReverbWidget", "medium", (
        "mittel", "medio", "moyen", "ミディアム", "médio", "中等", "中等", "mellem", "μέτριο", "meán", "middels", "średni", "средний", "keskitaso", "medium", "canolig", "middlin’",
    )),
    ("ReverbWidget", "Mod depth: %1%", (
        "Mod-Tiefe: %1%", "Profundidad mod: %1%", "Profondeur de mod : %1%", "変調の深さ: %1%", "Profundidade mod: %1%", "调制深度：%1%", "調制深度：%1%", "Mod-dybde: %1%", "Βάθος διαμόρφωσης: %1%", "Doimneas mod: %1%", "Mod-dybde: %1%", "Głębokość mod: %1%", "Глубина модуляции: %1%", "Modulaatiosyvyys: %1%", "Modulationsdjup: %1%", "Dwfn mod: %1%", "Mod depth: %1%",
    )),
    ("ReverbWidget", "Mod rate: %1 Hz", (
        "Mod-Rate: %1 Hz", "Velocidad mod: %1 Hz", "Vitesse de mod : %1 Hz", "変調レート: %1 Hz", "Taxa mod: %1 Hz", "调制速率：%1 Hz", "調制速率：%1 Hz", "Mod-hastighed: %1 Hz", "Ρυθμός διαμόρφωσης: %1 Hz", "Ráta mod: %1 Hz", "Mod-hastighet: %1 Hz", "Tempo mod: %1 Hz", "Скорость модуляции: %1 Гц", "Modulaation nopeus: %1 Hz", "Mod-hastighet: %1 Hz", "Cyflymder mod: %1 Hz", "Mod rate: %1 Hz",
    )),
    ("ReverbWidget", "Delay before the reverb tail begins (ms)", (
        "Verzögerung vor Beginn des Halltails (ms)", "Retraso antes de la cola de reverb (ms)", "Délai avant la queue de réverb (ms)", "リバーブテール開始までの遅延（ms）", "Atraso antes da cauda do reverb (ms)", "混响尾部开始前的延迟（毫秒）", "殘響尾音開始前的延遲（毫秒）", "Forsinkelse før rumklangs-hale (ms)", "Καθυστέρηση πριν την ουρά hall (ms)", "Moill roimh eireaball an athfhuaimnithe (ms)", "Utsetter før romklang-hale (ms)", "Opóźnienie przed początkiem ogona reverba (ms)", "Задержка перед хвостом реверба (мс)", "Viive ennen kaiun häntää (ms)", "Fördröjning före reverb-svans (ms)", "Oedi cyn dechrau cynffon adlais (ms)", "Delay afore the tail (ms)",
    )),
    ("ReverbWidget", "Reverb decay time (seconds ×10)", (
        "Hall-Abklingzeit (Sekunden ×10)", "Tiempo de decaimiento del reverb (segundos ×10)", "Temps de déclin de la réverb (secondes ×10)", "リバーブ減衰時間（秒×10）", "Tempo de decay do reverb (segundos ×10)", "混响衰减时间（秒×10）", "殘響衰減時間（秒×10）", "Rumklang dempetid (sekunder ×10)", "Χρόνος εξασθένησης ηχούς (δευτερόλεπτα ×10)", "Am lagraithe athfhuaimnithe (soicindí ×10)", "Romklang utløping (sekunder ×10)", "Czas zanikania pogłosu (sekundy ×10)", "Время затухания реверберации (с ×10)", "Kaiun vaimennusaika (s ×10)", "Reverb-urslockning (sekunder ×10)", "Amser dirwyn adlais (eiliadau ×10)", "Reverb decay (seconds ×10)",
    )),
    ("ReverbWidget", "Wet/dry mix: 0% = dry, 100% = wet", (
        "Nass/Trocken-Mix: 0 % trocken, 100 % nass", "Mezcla húmedo/seco: 0 % = seco, 100 % = húmedo", "Mix humide/sec : 0 % = sec, 100 % = humide", "ウェット/ドライ：0%=ドライ、100%=ウェット", "Mistura molhado/seco: 0 % = seco, 100 % = molhado", "湿/干混合：0% 干，100% 湿", "濕/乾混合：0% 乾，100% 濕", "Våd/tør mix: 0 % = tør, 100 % = våd", "Υγρό/στεγρό μείγμα: 0 % = στεγνό, 100 % = υγρό", "Meascán fliuch/tirim: 0 % = tirim, 100 % = fliuch", "Våt/tørr miks: 0 % = tørr, 100 % = våt", "Miks mokry/suchy: 0 % = suchy, 100 % = mokry", "Смесь wet/dry: 0 % сухо, 100 % wet", "Märkä/kuiva -miks: 0 % kuiva, 100 % märkä", "Våt/torr mix: 0 % = torr, 100 % = våt", "Cymysg gwlyb/sych: 0 % = sych, 100 % = gwlyb", "Wet/dry: 0 % sober, 100 % sozzled",
    )),
    ("ReverbWidget", "High-frequency presence in the reverb tail", (
        "Höhenanteil im Halltail", "Presencia de agudos en la cola de reverb", "Présence des aigus dans la queue de réverb", "リバーブテールにおける高域の存在感", "Presença de agudos na cauda do reverb", "混响尾部的高频存在感", "殘響尾音的高頻存在感", "Højfrekvent tilstedeværelse i rumklangs-hale", "Υψηλές συχνότητες στην ουρά ηχούς", "Ceisteas hard-toinne i dtut eireaball athfhuaimnithe", "Høyfrekvent tilstedeværelse i romklang-hale", "Obecność wysokich częstotliwości w ogonie pogłosu", "ВЧ-присутствие в хвосте реверберации", "Korkeat taajuudet kaiun hännässä", "Diskant i reverb-svansen", "Presenoldeb uchffreuddiant yng nghynffon yr adlais", "High treble in the tail",
    )),
    ("ReverbWidget", "Modulation depth (motion in the tail)", (
        "Modulationstiefe (Bewegung im Tail)", "Profundidad de modulación (movimiento en la cola)", "Profondeur de modulation (mouvement dans la queue)", "変調の深さ（テール内の動き）", "Profundidade de modulação (movimento na cauda)", "调制深度（尾音中的运动）", "調制深度（尾音中的動態）", "Modulationsdybde (bevægelse i halen)", "Βάθος διαμόρφωσης (κίνηση στην ουρά)", "Doimneas modhnaithe (gluaiseacht san eireaball)", "Modulasjonsdybde (bevegelse i halen)", "Głębokość modulacji (ruch w ogonie)", "Глубина модуляции (движение в хвосте)", "Modulaatiosyvyys (liike hännässä)", "Modulationsdjup (rörelse i svansen)", "Dwfn cyfaddasu (symudiad yn y gynffon)", "Mod depth (swell in the tail)",
    )),
    ("ReverbWidget", "Modulation speed (Hz ×10, e.g. 4 = 0.4 Hz)", (
        "Modulationsgeschwindigkeit (Hz ×10, z. B. 4 = 0,4 Hz)", "Velocidad de modulación (Hz ×10, p. ej. 4 = 0,4 Hz)", "Vitesse de modulation (Hz ×10, ex. 4 = 0,4 Hz)", "変調速度（Hz ×10、例: 4 = 0.4 Hz）", "Velocidade de modulação (Hz ×10, ex.: 4 = 0,4 Hz)", "调制速度（Hz ×10，例如 4 = 0.4 Hz）", "調制速度（Hz ×10，例如 4 = 0.4 Hz）", "Modulationshastighed (Hz ×10, fx. 4 = 0,4 Hz)", "Ταχύτητα διαμόρφωσης (Hz ×10, π.χ. 4 = 0,4 Hz)", "Luas modhnaithe (Hz ×10, m.sh. 4 = 0.4 Hz)", "Modulasjonshastighet (Hz ×10, f.eks. 4 = 0,4 Hz)", "Prędkość modulacji (Hz ×10, np. 4 = 0,4 Hz)", "Скорость модуляции (Гц ×10, напр. 4 = 0,4 Гц)", "Modulaationopeus (Hz ×10, esim. 4 = 0,4 Hz)", "Modulationshastighet (Hz ×10, t.ex. 4 = 0,4 Hz)", "Cyflymder cyfaddasu (Hz ×10, e.e. 4 = 0.4 Hz)", "Mod speed (Hz ×10, e.g. 4 = 0.4 Hz)",
    )),
    ("TrackWidget", "Effects", (
        "Effekte", "Efectos", "Effets", "エフェクト", "Efeitos", "效果", "效果", "Effekter", "Εφέ", "Éifeachtaí", "Effekter", "Efekty", "Эффекты", "Tehosteet", "Effekter", "Effeithiau", "Effects",
    )),
    ("TrackWidget", "Configure insert effects for recordings on this audio track.", (
        "Insert-Effekte für Aufnahmen auf dieser Audiospur konfigurieren.",
        "Configurar efectos insertados para grabaciones en esta pista de audio.",
        "Configurer les effets d’insertion pour les enregistrements sur cette piste audio.",
        "このオーディオトラックの録音用インサートエフェクトを設定します。",
        "Configurar efeitos de inserção para gravações nesta faixa de áudio.",
        "配置此音频轨道上录音的插入效果。",
        "設定此音訊軌道上錄音的插入效果。",
        "Konfigurér insert-effekter til optagelser på dette lydspor.",
        "Ρυθμίστε εφέ εισαγωγής για εγγραφές σε αυτό το κομμάτι ήχου.",
        "Cumraigh éifeachtaí ionsáite le haghaidh taifeadta ar an rian fuaime seo.",
        "Konfigurer insert-effekter for opptak på dette lydsporet.",
        "Skonfiguruj efekty insert dla nagrań na tej ścieżce audio.",
        "Настроить insert-эффекты для записи на этой аудиодорожке.",
        "Määritä insert-tehosteet tallennuksille tällä ääniuralla.",
        "Konfigurera insert-effekter för inspelningar på detta ljudspår.",
        "Ffurfwch effeithiau mewnosod ar gyfer recordiadau ar y trac sain hwn.",
        "Set yer insert effects for recordin’ on this here audio track.",
    )),
]
# fmt: on


def main() -> None:
    out = Path(__file__).with_name("effect_i18n.tsv")
    with out.open("w", encoding="utf-8", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n")
        w.writerow(["context", "source", *LANGS])
        for ctx, src, trans in RAW:
            if len(trans) != len(LANGS):
                raise SystemExit(f"wrong len {ctx} {src}: {len(trans)} != {len(LANGS)}")
            w.writerow([ctx, src, *trans])
    print("Wrote", out)


if __name__ == "__main__":
    main()
