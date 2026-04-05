#!/usr/bin/env python3
"""Generate HTML user manual from Markdown source.

Usage:
    python3 generate_html.py              # Generate English (default)
    python3 generate_html.py de           # Generate German
    python3 generate_html.py all          # Generate all available languages
"""

import markdown
import re
import os
import sys

# Language configuration: suffix -> (lang code, title, CJK font flag)
LANGUAGES = {
    '':      ('en', "Musician&#8217;s Canvas User Manual", False),
    '_de':   ('de', "Musician&#8217;s Canvas Benutzerhandbuch", False),
    '_pt-BR':('pt-BR', "Manual do Usu&aacute;rio do Musician&#8217;s Canvas", False),
    '_fr':   ('fr', "Manuel d&#8217;utilisation de Musician&#8217;s Canvas", False),
    '_zh-TW':('zh-TW', "Musician&#8217;s Canvas &#20351;&#29992;&#25163;&#20874;", True),
    '_zh-CN':('zh-CN', "Musician&#8217;s Canvas &#29992;&#25143;&#25163;&#20876;", True),
    '_ja':   ('ja', "Musician&#8217;s Canvas &#12518;&#12540;&#12470;&#12540;&#12510;&#12491;&#12517;&#12450;&#12523;", True),
    '_es':   ('es', "Manual de usuario de Musician&#8217;s Canvas", False),
    '_da':   ('da', "Musician&#8217;s Canvas Brugermanual", False),
    '_el':   ('el', "Musician&#8217;s Canvas &#917;&#947;&#967;&#949;&#953;&#961;&#943;&#948;&#953;&#959; &#935;&#961;&#942;&#963;&#964;&#951;", False),
    '_ga':   ('ga', "Tr&#233;oirleabhar &#218;s&#225;ideora Musician&#8217;s Canvas", False),
    '_nb':   ('nb', "Musician&#8217;s Canvas Brukermanual", False),
    '_pl':   ('pl', "Podr&#281;cznik u&#380;ytkownika Musician&#8217;s Canvas", False),
    '_ru':   ('ru', "Musician&#8217;s Canvas &#1056;&#1091;&#1082;&#1086;&#1074;&#1086;&#1076;&#1089;&#1090;&#1074;&#1086; &#1087;&#1086;&#1083;&#1100;&#1079;&#1086;&#1074;&#1072;&#1090;&#1077;&#1083;&#1103;", False),
    '_fi':   ('fi', "Musician&#8217;s Canvas K&#228;ytt&#246;opas", False),
    '_sv':   ('sv', "Musician&#8217;s Canvas Anv&#228;ndarmanual", False),
    '_cy':   ('cy', "Llawlyfr Defnyddiwr Musician&#8217;s Canvas", False),
    '_pirate': ('en', "Musician&#8217;s Canvas &#8212; A Pirate&#8217;s Guide", False),
}

# Map CLI arg to suffix
ARG_TO_SUFFIX = {
    'en': '', 'de': '_de', 'pt-BR': '_pt-BR', 'pt-br': '_pt-BR',
    'fr': '_fr', 'zh-TW': '_zh-TW', 'zh-tw': '_zh-TW',
    'zh-CN': '_zh-CN', 'zh-cn': '_zh-CN', 'ja': '_ja',
    'es': '_es',
    'da': '_da', 'el': '_el', 'ga': '_ga', 'nb': '_nb',
    'pl': '_pl', 'ru': '_ru', 'fi': '_fi', 'sv': '_sv', 'cy': '_cy',
    'pirate': '_pirate',
}

script_dir = os.environ.get('SCRIPT_DIR', os.path.dirname(os.path.abspath(__file__)))
project_dir = os.environ.get('PROJECT_DIR', os.path.dirname(script_dir))
html_dir = os.path.join(script_dir, 'html')


def generate_html_for_lang(suffix):
    """Generate HTML for a single language variant."""
    lang_code, html_title, is_cjk = LANGUAGES[suffix]

    source = os.path.join(script_dir, f'MusiciansCanvas_User_Manual{suffix}.md')
    if not os.path.isfile(source):
        print(f'  Skipping {lang_code}: {source} not found')
        return False

    output = os.path.join(html_dir, f'MusiciansCanvas_User_Manual{suffix}.html')

    with open(source, 'r', encoding='utf-8') as f:
        raw = f.read()

    # Get version and date from environment or YAML frontmatter
    version = os.environ.get('MC_VERSION', '')
    date = os.environ.get('MC_DATE', '')
    if not version or not date:
        fm_match = re.match(r'^---\n(.*?)\n---\n', raw, flags=re.DOTALL)
        if fm_match:
            fm = fm_match.group(1)
            if not version:
                vm = re.search(r'version:\s*"?([^"\n]+)"?', fm)
                if vm: version = vm.group(1).strip()
            if not date:
                dm = re.search(r'date:\s*"?([^"\n]+)"?', fm)
                if dm: date = dm.group(1).strip()
    if not version: version = "0.1.0"
    if not date: date = "2026-04-03"

    # Read subtitle from frontmatter
    subtitle = "Multi-Track Music Recording Application"
    fm_match = re.match(r'^---\n(.*?)\n---\n', raw, flags=re.DOTALL)
    if fm_match:
        fm = fm_match.group(1)
        sm = re.search(r'subtitle:\s*"?([^"\n]+)"?', fm)
        if sm: subtitle = sm.group(1).strip()

    # Strip YAML frontmatter
    content = re.sub(r'^---\n.*?\n---\n', '', raw, flags=re.DOTALL)

    # Fix image paths: ../screenshots/ -> screenshots/ (since we copy them into html/)
    content = content.replace('](../screenshots/', '](screenshots/')

    md = markdown.Markdown(extensions=['tables', 'toc', 'fenced_code'],
                           extension_configs={'toc': {'toc_depth': 3}})
    html_body = md.convert(content)
    toc = md.toc

    # CJK font stack for Chinese/Japanese
    if is_cjk:
        font_family = ('"Noto Sans CJK SC", "Noto Sans CJK TC", "Noto Sans CJK JP", '
                       '"Microsoft YaHei", "PingFang SC", "Hiragino Sans", "Meiryo", '
                       '-apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif')
    else:
        font_family = '-apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif'

    css = f"""
body {{
    font-family: {font_family};
    line-height: 1.6; max-width: 900px; margin: 0 auto; padding: 20px 40px;
    color: #24292e; background-color: #fff;
}}
h1 {{ border-bottom: 2px solid #0366d6; padding-bottom: 0.3em; color: #0366d6; }}
h2 {{ border-bottom: 1px solid #eaecef; padding-bottom: 0.3em; margin-top: 2em; }}
h3 {{ color: #0366d6; margin-top: 1.5em; }}
h4 {{ color: #586069; }}
table {{ border-collapse: collapse; width: 100%; margin: 1em 0; }}
th, td {{ border: 1px solid #dfe2e5; padding: 8px 12px; text-align: left; }}
th {{ background-color: #f6f8fa; font-weight: 600; }}
tr:nth-child(even) {{ background-color: #f6f8fa; }}
code {{ background-color: #f6f8fa; padding: 0.2em 0.4em; border-radius: 3px; font-size: 0.9em;
       font-family: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, monospace; }}
pre {{ background-color: #f6f8fa; padding: 16px; border-radius: 6px; overflow-x: auto; }}
pre code {{ background: none; padding: 0; }}
strong {{ color: #24292e; }}
a {{ color: #0366d6; text-decoration: none; }}
a:hover {{ text-decoration: underline; }}
img {{ max-width: 100%; height: auto; border: 1px solid #dfe2e5; border-radius: 6px; margin: 1em 0; }}
.toc {{ background-color: #f6f8fa; border: 1px solid #dfe2e5; border-radius: 6px;
       padding: 15px 20px; margin: 1em 0 2em 0; }}
.toc h2 {{ margin-top: 0; border-bottom: none; font-size: 1.1em; }}
.toc ul {{ list-style: none; padding-left: 1.2em; }}
.toc > ul {{ padding-left: 0; }}
.toc li {{ margin: 0.3em 0; }}
.title-block {{ text-align: center; margin-bottom: 2em; padding-bottom: 1em;
               border-bottom: 2px solid #0366d6; }}
@media print {{
    body {{ max-width: none; padding: 0; }}
    h1, h2, h3 {{ page-break-after: avoid; }}
    table, pre, img {{ page-break-inside: avoid; }}
    img {{ max-width: 80%; }}
}}
"""

    # Table of contents header per language
    toc_headers = {
        'en': 'Table of Contents', 'de': 'Inhaltsverzeichnis',
        'es': '&Iacute;ndice', 'da': 'Indholdsfortegnelse',
        'el': '&#928;&#943;&#957;&#945;&#954;&#945;&#962; &#928;&#949;&#961;&#953;&#949;&#967;&#959;&#956;&#941;&#957;&#969;&#957;',
        'ga': 'Cl&aacute;r &Aacute;bhair', 'nb': 'Innholdsfortegnelse',
        'pl': 'Spis tre&sacute;ci', 'ru': '&#1057;&#1086;&#1076;&#1077;&#1088;&#1078;&#1072;&#1085;&#1080;&#1077;',
        'fi': 'Sis&auml;llysluettelo', 'sv': 'Inneh&aring;llsf&ouml;rteckning',
        'cy': 'Tabl Cynnwys',
        'pt-BR': '&Iacute;ndice', 'fr': 'Table des mati&egrave;res',
        'zh-TW': '&#30446;&#37636;', 'zh-CN': '&#30446;&#24405;',
        'ja': '&#30446;&#27425;',
    }
    toc_header = toc_headers.get(lang_code, 'Table of Contents')

    html = f'''<!DOCTYPE html>
<html lang="{lang_code}">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>{html_title}</title>
<style>{css}</style>
</head>
<body>
<div class="title-block">
<h1>{html_title}</h1>
<p>{subtitle} &mdash; Version {version}</p>
<p>{date}</p>
</div>
<div class="toc">
<h2>{toc_header}</h2>
{toc}
</div>
{html_body}
</body>
</html>'''

    os.makedirs(html_dir, exist_ok=True)
    with open(output, 'w', encoding='utf-8') as f:
        f.write(html)
    print(f'  HTML generated ({lang_code}): {output} ({len(html)} bytes)')
    return True


def main():
    arg = sys.argv[1] if len(sys.argv) > 1 else os.environ.get('MC_DOC_LANG', 'en')

    if arg == 'all':
        suffixes = list(LANGUAGES.keys())
    elif arg in ARG_TO_SUFFIX:
        suffixes = [ARG_TO_SUFFIX[arg]]
    else:
        # Default to English
        suffixes = ['']

    for suffix in suffixes:
        generate_html_for_lang(suffix)


if __name__ == '__main__':
    main()
