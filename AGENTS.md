# AGENTS.md

Guidance for coding agents and contributors working in this repository.

## Skills

- `.agents/music-theory-sync/SKILL.md` — changes to `src/music_theory/music_theory.hpp` must be synchronized with mnxdom.

## Project Context

This repository implements a C++ document object model for Finale `musx` / EnigmaXml data. Prefer changes that preserve the existing DOM/factory/test structure and keep Finale-specific interpretation in musxdom while leaving export-format policy decisions to caller layers.

## Repository Map

- `src/musx/dom/`: core DOM types and interpretation helpers. Major files include `Others.*`, `Details.*`, `Entries.*`, `Staff.*`, `Options.*`, `Document.*`, and shared base/object-pool classes.
- `src/musx/factory/`: XML-to-DOM construction, field populators, type registry, resolver hooks, and parser-independent factory logic.
- `src/musx/xml/`: parser abstraction interfaces and implementations for supported XML backends.
- `src/musx/util/`: reusable interpretation and conversion utilities, including fractions, layout helpers, shapes, ties, SVG conversion, logging, and test support.
- `src/music_theory/`: music-theory support code used by the DOM.
- `tests/`: GoogleTest coverage organized by domain (`others`, `details`, `entries`, `options`, `dom`, `util`, and related groups).
- `tests/data/`: EnigmaXml, musx, SVG, font metadata, and reference fixtures. Many tests expect this directory to be the process working directory.
- `docs/`: reserved for the Doxygen website, including Doxygen configuration and generated-documentation support files. Do not put incidental project notes, todo lists, design scratchpads, or agent working docs here; place those elsewhere.
- `third_party/`: vendored lightweight dependencies used by the library.
- `finale/`: Finale-related static reference data.

## Code Style

- Follow the surrounding C++ style in the file being edited.
- Keep changes focused. Avoid unrelated refactors while fixing or adding a behavior.
- Prefer existing DOM helper APIs such as `Document`, `ObjectPool`, `MusxInstance`, `StaffComposite`, and `GFrameHoldContext` over duplicating traversal logic.
- Except in rare circumstances confirmed by the user, hard-code only the numeric value `0`; use
  named constants for other fixed values. Mathematical operations and mathematical formulas are an
  exception and do not require separate confirmation.
- Doxygen comments document the contract, not its history. State what a caller must know:
  behavior, parameters, return values, and what is thrown. Do not explain why a function or
  property was added, what it replaced, or which problem prompted it. A reader arriving at the
  published API has none of that context, and the rationale ages badly once the surrounding code
  moves on. Such explanation belongs in an ordinary implementation comment, a commit message, or
  a design note.
- Null-check anything that should never be null short of a program bug, input parameters
  especially, with `MUSX_ASSERT_IF`, and throw from its body. The assertion halts a debug build at
  the fault, and the throw stops a release build from continuing on a bad pointer. Reserve a
  sentinel or an empty `std::optional` for outcomes a caller can legitimately encounter; a
  violated precondition is not one of them. A parameter that genuinely accepts null is the
  exception and must say so at the declaration, so that the absence of a check reads as deliberate
  rather than forgotten.
- Use `MUSX_INTEGRITY_ERROR` for malformed document relationships that existing code treats as integrity failures.
- For future variable, parameter, field, and method-local names representing `...Cmper` identifiers, prefer names ending in `Id`, such as `measureId`, `partId`, `staffId`, `systemId`, and `pageId`.

## Tests

- Build with:

```bash
cmake --build build
```

- Run tests with CTest so CMake supplies the correct working directory:

```bash
ctest --test-dir build/tests --output-on-failure
```

- Tests that read fixture files assume the process current directory is `tests/data`. `musxtest::getInputPath()` returns `std::filesystem::current_path()`, so running the test executable from the repository root will make fixture lookups fail.
- If running `musxdomtests` directly, set the working directory to `tests/data`, for example:

```bash
cd tests/data
../../build/tests/musxdomtests --gtest_filter=StaffTest.*
```

## Test Data

- Keep generated or temporary test outputs under the configured test output location, normally relative to `tests/data`.
- Do not modify large reference fixtures unless the behavior change genuinely requires it.

## Git Hygiene

- The worktree may contain unrelated user edits. Do not revert files you did not intentionally change.
- Before finishing, check `git status --short` and make clear which files your work touched.
