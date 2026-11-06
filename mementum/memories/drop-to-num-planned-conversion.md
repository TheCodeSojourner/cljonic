---
type: Decision
symbol: 🎯
title: Drop to_num from the planned conversion family
---

Human decision 2026-11-06: remove `to_num` from the planned cljonic API. C++ has no universal built-in Number destination corresponding to Clojure's numeric object model, and no useful cljonic-specific conversion contract was identified.

Removed the name from REQ-NUM-017 and the Core cheatsheet. The remaining planned conversion names are unchanged. This decision does not approve a generic destination-parameterized conversion API or change implicit conversion admission or equality behavior; those ideas remain discussion proposals.
