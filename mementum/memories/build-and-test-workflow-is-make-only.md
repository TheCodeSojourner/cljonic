---
type: Decision
symbol: 🎯
title: build-and-test-workflow-is-make-only
related:
  - /mementum/knowledge/make-target-strategy.md
---
Human decision, 2026-11-10: this repository's supported build and test workflow
uses its Make targets. Do not look for, mention, or use the VS Code CMake Tools
extension in this repository, including when investigating editor build errors.
Use Make output and repository build configuration to diagnose build or test
problems.
