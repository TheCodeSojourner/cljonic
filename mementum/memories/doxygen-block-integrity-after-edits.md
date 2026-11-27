---
type: Mistake
symbol: ❌
title: preserve-doxygen-block-integrity-after-edits
related: [doc-sample-extraction-requires-unindented-fence.md, docs-user-facing-example-style.md]
---
After changing a Doxygen comment, verify the complete bare `~~~~~{.cpp}` block before proceeding.

A malformed patch can duplicate or truncate nearby lines and introduce invalid tokens. Preserve the whole block, format it once, regenerate
the amalgamated header, extract the example, compile it, link it, and execute it. For an `assoc_in` correction, target
`build/docs-examples/cljonic-assoc-in-example-1.cpp` and `cljonic-can-assoc-in-example-1.cpp`; source extraction alone is insufficient.
