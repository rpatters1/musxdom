---
name: nested-namespaces
description: Use when adding, editing, reviewing, or refactoring C++ namespace declarations in this repository. Requires nested namespace blocks instead of C++17 concatenated namespace declarations.
---

# Nested Namespaces

Use nested namespace blocks for every namespace declaration. The opening declaration, namespace content, and closing comment must be on separate lines, using the form `namespace xxx {`, `<content>`, and `} // xxx`:

```cpp
namespace musx {
namespace factory {

// Declarations and definitions.

} // namespace factory
} // namespace musx
```

Do not write concatenated declarations such as:

```cpp
namespace musx::factory {
```

Apply this rule to source files, headers, tests, and any other C++ code changed in the repository. Preserve the surrounding namespace-block style when editing existing code.

When closing nested blocks, put the closing comment on the same line as the closing brace and include the namespace name for readability.
