#!/usr/bin/env python3
"""Fill effect-related Qt Linguist entries from scripts/effect_i18n.tsv into musicians_canvas_*.ts."""

from __future__ import annotations

import csv
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TRANS_DIR = ROOT / "translations"
TSV_PATH = Path(__file__).with_name("effect_i18n.tsv")

TARGET_CONTEXTS = frozenset(
    {
        "ChorusWidget",
        "ReverbWidget",
        "FlangeWidget",
        "EffectsDialog",
        "EffectWidget",
        "TrackWidget",
    }
)

# TSV column header -> TS file language="..." attribute
HEADER_TO_LANG = {
    "de_DE": "de_DE",
    "es_ES": "es_ES",
    "fr_FR": "fr_FR",
    "ja_JP": "ja_JP",
    "pt_BR": "pt_BR",
    "zh_CN": "zh_CN",
    "zh_TW": "zh_TW",
    "da_DK": "da_DK",
    "el_GR": "el_GR",
    "ga_IE": "ga_IE",
    "nb_NO": "nb_NO",
    "pl_PL": "pl_PL",
    "ru_RU": "ru_RU",
    "fi_FI": "fi_FI",
    "sv_SE": "sv_SE",
    "cy_GB": "cy_GB",
    "en_PIRATE": "en_PIRATE",
}


def load_rows() -> list[dict[str, str]]:
    if not TSV_PATH.is_file():
        print(f"Missing {TSV_PATH}", file=sys.stderr)
        sys.exit(1)
    text = TSV_PATH.read_text(encoding="utf-8")
    # Allow optional BOM
    if text.startswith("\ufeff"):
        text = text[1:]
    reader = csv.DictReader(text.splitlines(), delimiter="\t")
    rows = []
    for row in reader:
        if not row.get("context"):
            continue
        rows.append(row)
    return rows


def build_lookup(rows: list[dict[str, str]]) -> dict[str, dict[str, dict[str, str]]]:
    """lang -> context -> source -> translation"""
    out: dict[str, dict[str, dict[str, str]]] = {}
    for col, lang in HEADER_TO_LANG.items():
        out[lang] = {}
    for row in rows:
        ctx = row["context"].strip()
        src = row["source"]
        if not src:
            continue
        for col, lang in HEADER_TO_LANG.items():
            val = row.get(col, "").strip()
            if not val:
                continue
            out[lang].setdefault(ctx, {})[src] = val
    return out


def apply_to_ts(ts_path: Path, lang: str, lookup: dict[str, dict[str, dict[str, str]]]) -> int:
    if lang not in lookup:
        return 0
    per_lang = lookup[lang]
    tree = ET.parse(ts_path)
    root = tree.getroot()
    updated = 0
    for ctx_el in root.findall("context"):
        name_el = ctx_el.find("name")
        if name_el is None or name_el.text is None:
            continue
        cname = name_el.text.strip()
        if cname not in TARGET_CONTEXTS or cname not in per_lang:
            continue
        ctx_map = per_lang[cname]
        for msg_el in ctx_el.findall("message"):
            src_el = msg_el.find("source")
            trans_el = msg_el.find("translation")
            if src_el is None or trans_el is None or src_el.text is None:
                continue
            src = src_el.text
            if src not in ctx_map:
                continue
            trans_el.text = ctx_map[src]
            if trans_el.attrib.get("type") == "unfinished":
                del trans_el.attrib["type"]
            updated += 1
    ET.indent(tree, space="    ")
    tree.write(ts_path, encoding="utf-8", xml_declaration=True)
    return updated


def main() -> None:
    rows = load_rows()
    lookup = build_lookup(rows)
    for ts_path in sorted(TRANS_DIR.glob("musicians_canvas_*.ts")):
        tree = ET.parse(ts_path)
        lang = tree.getroot().get("language")
        if not lang:
            continue
        n = apply_to_ts(ts_path, lang, lookup)
        print(f"{ts_path.name} ({lang}): {n} messages")


if __name__ == "__main__":
    main()
