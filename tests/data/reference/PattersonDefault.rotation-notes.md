# PattersonDefault Rotation Notes

This fixture was intentionally edited on 2026-02-14 to include the alternate packed-angle rotation encoding on two shapes.

Purpose:
- Keep a stable regression sample that exercises both observed rotation encodings.
- Preserve a known case where conversion math is numerically stable but not bit-identical to legacy packed values.

## Shape Mapping

- `shapeDef cmper=129` -> `instList=23`, `dataList=23`
- `shapeDef cmper=132` -> `instList=47`, `dataList=47`

## Raw Value Updates

These are the exact rotation slots changed in shape data:

- `cmper=23, inci=2, slot=2`: `0x000001E3` -> `0x2000BFDF` (`483` -> `536920031`)
- `cmper=23, inci=10, slot=0`: `0x800002D4` -> `0x2003986E` (`-2147482924` -> `537106542`)
- `cmper=23, inci=17, slot=1`: `0xC00002ED` -> `0x20060BAB` (`-1073741075` -> `537267115`)
- `cmper=23, inci=24, slot=2`: `0x40000210` -> `0x2008C2C0` (`1073742352` -> `537445056`)
- `cmper=47, inci=2, slot=2`: `0xC000022E` -> `0x2005AC4C` (`-1073741266` -> `537242700`)

## Expected SVG Impact

Only tiny numeric coordinate drift is expected (typically `0.001` to `0.002`), with no visible rendering change.

Reference files known to differ:
- `tests/data/reference/PattersonDefault/129.svg`
- `tests/data/reference/PattersonDefault/132.svg`
- `tests/data/reference/PattersonDefault_page_scaled/132.svg`

## Write Method

These updates were applied with a bespoke one-off C++ plugin that writes raw shape-data
incidences (`ot_SB`) directly.

Why this matters:
- Lua-level shape setters did not persist these values in this workflow.
- Direct raw inci writes were required to preserve the alternate packed-angle encoding.

Implementation reference (outside this repo):
- `DoOneOffFunction` in
  `.../RGP PlugIns/Force Full Names/-Source/FullName.cpp`
