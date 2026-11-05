---
type: Decision
symbol: 🎯
title: doxygen-examples-import-root-namespace
related: [doxygen-examples-use-free-functions.md]
---

Every active C++ example in `src/*.hpp` uses `using namespace cljonic;` at file
scope after its includes. This is the idiomatic cljonic style for collection
and producer types; retain `using namespace cljonic::core;` where examples use
Core functions. Do not replace the root directive with selective
`using cljonic::Type;` declarations in Doxygen examples. The
`docs-examples` validator enforces the root import and its file-scope
placement.
