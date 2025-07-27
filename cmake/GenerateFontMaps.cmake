# GenerateFontMaps.cmake
#
# Safe to use via FetchContent

# Locate base path relative to this file (e.g., /cmake/ → /src/musx/fontmaps)
get_filename_component(_gen_script_dir "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
set(FONTMAP_BASE_DIR "${_gen_script_dir}/../src/musx/fontmaps")

function(generate_fontmap_headers)
    set(FONTMAP_SOURCES_DIR "${FONTMAP_BASE_DIR}/sources")
    set(OUTPUT_DIR "${FONTMAP_BASE_DIR}/glyphnames")
    set(PY_SCRIPT "${FONTMAP_BASE_DIR}/tools/generate_glyphnames_map.py")

    file(MAKE_DIRECTORY "${OUTPUT_DIR}")

    set(INPUTS
        "${FONTMAP_SOURCES_DIR}/glyphnames.json|standardGlyphnames|${OUTPUT_DIR}/StandardGlyphnamesMap.h"
        "${FONTMAP_SOURCES_DIR}/glyphnamesFinale.json|finaleGlyphnames|${OUTPUT_DIR}/FinaleGlyphnamesMap.h"
    )

    foreach(entry IN LISTS INPUTS)
        # Split the '|' delimited string into parts
        string(REPLACE "|" ";" entry_parts "${entry}")

        list(GET entry_parts 0 input_json)
        list(GET entry_parts 1 var_name)
        list(GET entry_parts 2 output_header)

        add_custom_command(
            OUTPUT ${output_header}
            COMMAND ${CMAKE_COMMAND} -E echo "Generating ${output_header}"
            COMMAND ${Python3_EXECUTABLE} ${PY_SCRIPT} ${input_json} ${var_name} ${output_header}
            DEPENDS ${input_json} ${PY_SCRIPT}
            COMMENT "Generating ${output_header} from ${input_json}"
            VERBATIM
        )

        list(APPEND GENERATED_HEADERS ${output_header})
    endforeach()

    set(FONTMAP_GENERATED_HEADERS ${GENERATED_HEADERS} PARENT_SCOPE)
endfunction()
