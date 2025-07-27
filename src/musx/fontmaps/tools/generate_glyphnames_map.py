#!/usr/bin/env python3

import json
import sys
from pathlib import Path

def parse_codepoint(uplus):
    return int(uplus.replace("U+", ""), 16)

def escape_cpp_string(text):
    return text.replace("\\", "\\\\").replace("\"", "\\\"")

def generate_glyphnames_header(input_path: Path, var_name: str, output_path: Path):
    with open(input_path, "r", encoding="utf-8") as f:
        glyphs = json.load(f)

    entries = []
    for name, data in glyphs.items():
        codepoint = parse_codepoint(data.get("codepoint", "U+FFFD"))
        description = escape_cpp_string(data.get("description", ""))
        entries.append((name, codepoint, description))

    entries.sort()

    with open(output_path, "w", encoding="utf-8") as out:
        out.write("// This file is auto-generated from glyphnames.json\n")
        out.write("#pragma once\n\n")
        out.write("#include <unordered_map>\n#include <string_view>\n\n")
        out.write("namespace musx::fontmaps::glyphnames {\n\n")
        out.write("struct GlyphInfo {\n")
        out.write("    char32_t codepoint;\n")
        out.write("    std::string_view description;\n")
        out.write("};\n\n")
        out.write(f"inline const std::unordered_map<std::string_view, GlyphInfo> {var_name} = {{\n")

        for name, cp, desc in entries:
            out.write(f'    {{ "{name}", {{ 0x{cp:X}, "{desc}" }} }},\n')

        out.write("};\n\n} // namespace musx::fontmaps::glyphnames\n")

# Entry point
if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: generate_glyphnames_map.py <glyphnames.json> <varName> <output.h>")
        sys.exit(1)

    input_json = Path(sys.argv[1])
    var_name = sys.argv[2]
    output_header = Path(sys.argv[3])

    generate_glyphnames_header(input_json, var_name, output_header)
