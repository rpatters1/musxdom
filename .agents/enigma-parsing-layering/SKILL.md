---
name: enigma-parsing-layering
description: Use when modifying Enigma text parsing, `getRawTextCtx` call chains, `TextInsertCallback` handlers, or any code that substitutes tags like `^value`, `^control`, `^rehearsal`, `^page`, or `^partname`. This skill explains the layered fallback model and prevents replacing inner handlers when adding outer ones.
---

# Enigma Parsing Layering

Use this skill before changing any of:

- `util::EnigmaString::parseEnigmaText*`
- `util::EnigmaParsingContext`
- `TextsBase::getRawTextCtx`
- `TextBlock::getRawTextCtx`
- `TextExpressionDef::getRawTextCtx`
- `MeasureExprAssign::getRawTextCtx`
- other context-building functions that add insert handlers

## Core rule

Enigma insert handling is layered.

The outermost caller gets first access to a tag. It may override an inner default by returning a string, or it may defer by returning `std::nullopt` so the next inner layer can try. Replacing an inner context or bypassing its stored handler breaks inherited tags when deferral was intended.

## Dispatch order

For a context created by `getRawTextCtx(...)`, tag resolution runs in this order:

1. The `onInsert` callback passed to `EnigmaParsingContext::parseEnigmaText`.
2. The context's stored default handler `m_insertFunc`.
3. Context-local built-ins in `EnigmaParsingContext` itself, currently page-number handling for `^page`.
4. Generic built-ins in `EnigmaString::parseEnigmaTextImpl`.
5. Unknown-tag stripping or literal preservation, depending on parsing options.

If a layer returns a string, resolution stops there and that layer overrides any inner defaults for that tag.
If a layer wants the next layer to try, it may return `std::nullopt`.

## Important consequence

When adding a new outer handler, do not recreate the chain in a way that discards inner defaults.

Example:

- `TextExpressionDef::getRawTextCtx` adds default handling for `^value`, `^control`, and `^pass`.
- A measure-expression assignment layer may want to add `^rehearsal`.
- That outer layer may override those tags, or it may preserve the text-definition layer so `^value`, `^control`, and `^pass` still work.

If you overwrite the text definition's context instead of wrapping or extending it, those inner tags stop resolving.

## Required workflow before edits

1. Trace where the `EnigmaParsingContext` is first created.
2. List every handler added along the path from raw text to final caller.
3. Identify which layer owns each tag.
4. Only then add the new outer behavior.
5. Decide explicitly whether each affected tag should be overridden or deferred.

## Do not do this

- Do not replace a context-creation call with one that skips an existing default handler.
- Do not move a tag handler outward unless you intentionally mean to override the inner layer or preserve it via deferral.
- Do not treat `defaultInsertFunc` as a single global handler. It is one layer in a stack.
- Do not change `getRawTextCtx` code without tracing the full fallback order first.

## Read next

Read [references/layering.md](references/layering.md) for the code map and a concrete example of the layering chain in this repository.
