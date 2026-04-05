#!/bin/bash
#
# generate_docs.sh - Generate HTML and PDF user manual from Markdown source
#
# Prerequisites (Python - preferred):
#   pip3 install markdown weasyprint
#
# Fallback (system packages):
#   sudo apt-get install pandoc wkhtmltopdf  (Ubuntu/Debian)
#   brew install pandoc wkhtmltopdf          (macOS)
#
# Usage:
#   ./generate_docs.sh          # Generate all formats (HTML + PDF), all languages
#   ./generate_docs.sh html     # Generate HTML only, all languages
#   ./generate_docs.sh pdf      # Generate PDF only, all languages
#   ./generate_docs.sh all      # Generate all formats, all languages

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
HTML_DIR="$SCRIPT_DIR/html"

# Supported language suffixes (empty string = English)
LANG_SUFFIXES=("" "_de" "_es" "_pt-BR" "_fr" "_zh-TW" "_zh-CN" "_ja" "_da" "_el" "_ga" "_nb" "_pl" "_ru" "_fi" "_sv" "_cy" "_pirate")

# Export dirs for Python scripts
export SCRIPT_DIR
export PROJECT_DIR

# Extract version from CMakeLists.txt if available
CMAKE_FILE="$PROJECT_DIR/CMakeLists.txt"
if [ -f "$CMAKE_FILE" ]; then
    export MC_VERSION=$(grep 'project(musicians_canvas VERSION' "$CMAKE_FILE" | sed -E 's/.*VERSION ([0-9.]+).*/\1/')
    echo "Version: $MC_VERSION (from CMakeLists.txt)"
fi
export MC_DATE=$(date +%Y-%m-%d)

generate_html() {
    echo "Generating HTML user manuals..."
    mkdir -p "$HTML_DIR"

    # Copy screenshots into html/ so relative paths work from the HTML file
    if [ -d "$PROJECT_DIR/screenshots" ]; then
        mkdir -p "$HTML_DIR/screenshots"
        cp -u "$PROJECT_DIR/screenshots/"*.png "$HTML_DIR/screenshots/" 2>/dev/null || \
        cp "$PROJECT_DIR/screenshots/"*.png "$HTML_DIR/screenshots/" 2>/dev/null || true
        echo "  Copied screenshots to $HTML_DIR/screenshots/"
    fi

    if python3 -c "import markdown" 2>/dev/null; then
        python3 "$SCRIPT_DIR/generate_html.py" all
    elif command -v pandoc &>/dev/null; then
        # Pandoc fallback: generate each language individually
        for suffix in "${LANG_SUFFIXES[@]}"; do
            SOURCE="$SCRIPT_DIR/MusiciansCanvas_User_Manual${suffix}.md"
            HTML_OUTPUT="$HTML_DIR/MusiciansCanvas_User_Manual${suffix}.html"
            if [ ! -f "$SOURCE" ]; then
                echo "  Skipping ${suffix:-en}: $SOURCE not found"
                continue
            fi
            sed 's|](../screenshots/|](screenshots/|g' "$SOURCE" > "/tmp/mc_manual_tmp${suffix}.md"
            pandoc "/tmp/mc_manual_tmp${suffix}.md" \
                --from markdown \
                --to html5 \
                --standalone \
                --toc \
                --toc-depth=3 \
                --metadata title="Musician's Canvas User Manual" \
                -o "$HTML_OUTPUT"
            rm -f "/tmp/mc_manual_tmp${suffix}.md"
            echo "  HTML generated: $HTML_OUTPUT"
        done
    else
        echo "Error: Neither Python 'markdown' module nor 'pandoc' is available."
        echo "Install one of:"
        echo "  pip3 install markdown"
        echo "  sudo apt-get install pandoc"
        exit 1
    fi
}

generate_pdf() {
    echo "Generating PDF user manuals..."

    for suffix in "${LANG_SUFFIXES[@]}"; do
        HTML_FILE="$HTML_DIR/MusiciansCanvas_User_Manual${suffix}.html"
        PDF_FILE="$SCRIPT_DIR/MusiciansCanvas_User_Manual${suffix}.pdf"

        # Skip if HTML doesn't exist
        if [ ! -f "$HTML_FILE" ]; then
            echo "  Skipping PDF for ${suffix:-en}: HTML not found"
            continue
        fi

        if python3 -c "from weasyprint import HTML" 2>/dev/null; then
            python3 -c "
from weasyprint import HTML
import os
html_file = '${HTML_FILE}'
pdf_file = '${PDF_FILE}'
HTML(filename=html_file, base_url='${HTML_DIR}').write_pdf(pdf_file)
print(f'  PDF generated: {pdf_file}')
"
        elif command -v wkhtmltopdf &>/dev/null; then
            wkhtmltopdf \
                --enable-local-file-access \
                --page-size Letter \
                --margin-top 20mm \
                --margin-bottom 20mm \
                --margin-left 15mm \
                --margin-right 15mm \
                --header-center "Musician's Canvas User Manual" \
                --header-font-size 9 \
                --header-spacing 5 \
                --footer-center "[page] / [topage]" \
                --footer-font-size 9 \
                --footer-spacing 5 \
                "$HTML_FILE" \
                "$PDF_FILE" 2>/dev/null
            echo "  PDF generated: $PDF_FILE"
        else
            echo "Warning: Cannot generate PDF."
            echo "Install one of:"
            echo "  pip3 install weasyprint"
            echo "  sudo apt-get install wkhtmltopdf"
            return 1
        fi
    done
}

case "${1:-all}" in
    html)
        generate_html
        ;;
    pdf)
        # Ensure HTML exists first
        if ! ls "$HTML_DIR"/MusiciansCanvas_User_Manual*.html 1>/dev/null 2>&1; then
            generate_html
        fi
        generate_pdf
        ;;
    all)
        generate_html
        generate_pdf
        ;;
    *)
        echo "Usage: $0 [html|pdf|all]"
        exit 1
        ;;
esac

echo "Done."
