# Enigma Parsing Layering Reference

## Code map

- `TextsBase::getRawTextCtx(...)` constructs `EnigmaParsingContext` and stores the supplied `defaultInsertFunc`.
- `TextBlock::getRawTextCtx(...)` resolves the raw text object and forwards the handler into `TextsBase::getRawTextCtx(...)`.
- `TextExpressionDef::getRawTextCtx(...)` creates a text-block context whose default handler owns `^value`, `^control`, and `^pass`.
- `EnigmaParsingContext::parseEnigmaText(...)` runs:
  1. per-call `onInsert`
  2. stored `m_insertFunc`
  3. local page fallback for `^page`
  4. `EnigmaString::parseEnigmaTextImpl(...)`
- `EnigmaString::parseEnigmaTextImpl(...)` first asks `onInsert`, then applies generic built-ins such as file-info tags, `^page`, `^partname`, `^totpages`, date/time tags, accidental handling, and unknown-tag behavior.

## Practical ownership model

Think of each context-building function as adding one handler layer.

Typical chain:

1. Raw text exists in a `TextsBase` object.
2. `TextBlock::getRawTextCtx(...)` wraps it.
3. `TextExpressionDef::getRawTextCtx(...)` adds expression-definition defaults.
4. A caller such as a measure expression assignment may add assignment-specific tags.
5. The final caller of `parseEnigmaText(...)` may add one more temporary handler for the current operation.

The last-created layer gets first refusal and may either override or defer.

## What `std::nullopt` means

`std::nullopt` does not mean "empty replacement".

It means:

- "I do not handle this tag here."
- "Let the next inner layer try."

By contrast:

- `std::string{}` means "I handled it, and the replacement is empty."

That distinction is critical.

## Failure mode to avoid

Bad pattern:

1. Start from a context that already resolves `^value`.
2. Add a new `^rehearsal` handler by rebuilding the context incorrectly.
3. The rebuilt path no longer invokes the text definition's stored handler.
4. `^value`, `^control`, or `^pass` silently stop working.

This is only a bug when deferral was intended. If the outer layer intentionally overrides an inner tag, that is valid.

## Safe change checklist

Before changing any Enigma parsing layer:

1. Find the current context constructor chain.
2. Write down which layer handles which tags.
3. Confirm which tags should override and which should defer with `std::nullopt`.
4. Add tests for both the new tag and at least one inherited inner-layer tag.
5. Verify inherited tags still resolve after your change.

## Repository-specific warning

If you modify code around measure-expression or text-expression contexts, preserve the distinction between:

- expression-definition defaults
- assignment-level overrides
- per-call temporary handlers

That separation is the design, not an implementation detail.
