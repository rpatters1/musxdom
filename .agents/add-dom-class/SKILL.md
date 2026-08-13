---
name: add-dom-class
description: Add, extend, or review C++ DOM classes that model Finale musx/EnigmaXML records in musxdom. Use when introducing an options, others, details, entries, texts, or contained class; mapping new XML nodes or enums; registering a factory-created type; splitting a domain into a new header; or adding population tests for a DOM record.
---

# Add a musxdom DOM Class

Model the XML faithfully while presenting a clear, idiomatic public API. Keep Finale-specific
interpretation in musxdom and export-format policy in callers.

## 1. Establish the XML contract

1. Read `AGENTS.md` and inspect analogous classes, mappings, registrations, and tests.
2. Inspect multiple real fixtures when field presence, nesting, defaults, or meaning is uncertain.
   Use the installed `denigma export input.musx --enigmaxml output.enigmaxml` for compressed
   `.musx` files. Write decoded research output only to a temporary or ignored location.
3. Record the exact record category, XML node name, key attributes, child order, omitted fields,
   repeated children, boolean tags, and observed enum strings.
4. Do not infer a repeated array from separately named children such as `<layer1>` through
   `<layer4>`. Model the actual XML shape and use repository constants such as `MAX_LAYERS`.
5. Leave uncertain open-ended values as their lossless scalar type instead of inventing an enum.

## 2. Design the types

- Put the class in the domain header that owns the concept. Create a focused header when the
  concept warrants one; do not put playback data in `Staff.h` merely because it has a staff ID.
- Derive pooled records from the correct category base: `OptionsBase`, `OthersBase`,
  `DetailsBase`, `EntriesBase`, or `TextsBase`. Derive embedded owned records from
  `ContainedClassBase` and inherit its constructor.
- Give every pooled class its normal constructor, exact `XmlNodeName`, and `xmlMappingArray`
  declaration.
- Prefer semantic public names even when XML tags are abbreviated. Preserve the exact tag in the
  mapping and mention it in Doxygen when the names differ.
- Follow repository casing: `Id`, `Uuid`, `Midi`, and similar normal CamelCase forms rather than
  `ID`, `UUID`, or unexplained abbreviations. Expand cryptic names when their meaning is known.
- Hard-code only `0` unless `AGENTS.md` permits an exception. Reuse constants from
  `Fundamentals.h` and existing domain types such as `Cmper`, `StaffCmper`, and `LayerIndex`.
- Use pointers for optional contained objects when absence is meaningful. Missing XML fields
  retain value-initialized defaults.
- Declare XML-backed public fields in the order they are observed in the XML, including optional
  contained-object pointers. Preserve the same order in the field mapping so the class declaration
  and mapping jointly retain the information needed by a possible future serializer.
- Add `integrityCheck` only for a document invariant or relationship. Call the base check first,
  use `MUSX_INTEGRITY_ERROR` for malformed document data, and avoid silently changing valid data.

## 3. Write API documentation

Add Doxygen comments consistent with surrounding classes:

- State what the class represents, what its cmper/inci identifies, and its XML node name.
- Document enum semantics and every public member.
- Describe indexing and cross-record relationships precisely, including zero- versus one-based
  values.
- Document behavior and caller-visible contracts, not the history or rationale for the change.
- Do not claim semantics that the fixtures or established Finale behavior do not support.

## 4. Add factory mappings

Use the matching `FieldPopulators*.cpp` translation unit.

- Add enum mappings in the enum-mapping section and element mappings in the element-array section.
- Keep class names alphabetized within each section of every FieldPopulator translation unit.
  Group contained classes with their owner; place contained mappings immediately before the owner
  and alphabetize sibling contained-class names.
- Preserve observed XML child order inside each `MUSX_XML_ELEMENT_ARRAY` even though lookup is
  name-based. Mapping entries must have the same order as their corresponding fields in the class
  declaration. This supports eventual serialization.
- Map renamed API members from the original XML spelling explicitly.
- Use `populateBoolean`, `toEnum`, and `FieldPopulator<T>::createAndPopulate` rather than custom
  parsing where those helpers apply.
- Treat unknown XML children as evidence to investigate, not fields to suppress casually.

## 5. Register and expose the class

For factory-created pooled records:

1. Add the class to the appropriate `RegisteredTypes` list in `PoolFactory.cpp`.
2. Include a new domain header wherever the registry or implementation requires the complete type.
3. Expose a public domain header through `src/musx/musx.h`.
4. Add any new `.cpp` source to the explicit `MUSX_SRC_FILES` list in the root `CMakeLists.txt`.

Contained classes do not belong in `RegisteredTypes`; the owning class creates them through its
field mapping.

## 6. Add focused tests

- Add the test source to `tests/CMakeLists.txt`; tests are listed explicitly.
- Prefer a small inline EnigmaXML fixture that covers every new field, nested object, boolean,
  enum value, and renamed member.
- Test both present boolean tags and omitted/default-false behavior.
- Test absent optional children and default scalar values when relevant.
- Use a real existing fixture additionally when it proves a relationship that a synthetic fixture
  cannot, but do not modify large fixtures unnecessarily.
- Assert retrieval through the actual document pool so registration is tested, not just field
  population in isolation.

## 7. Verify and hand off

Run:

```bash
cmake --build build
ctest --test-dir build/tests --output-on-failure
git diff --check
git status --short
```

During iteration, a focused CTest regex is acceptable, but run proportionate final coverage.
Review the diff for section-local alphabetization, matching field/mapping order, exact XML
spelling, accidental hard-coded constants, public-header exposure, and unrelated edits. Do not
commit unless the user asks.
