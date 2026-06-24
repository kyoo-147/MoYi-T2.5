# Next Foundation Hardening Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Turn the MoYi scaffold into a verified native edge runtime foundation.

**Architecture:** Keep the C++ core as the source of truth. Add tests, asset loaders, deterministic CLI behavior, and benchmark/reporting surfaces before wiring production model inference.

**Tech Stack:** C++20, CMake/CTest, lightweight JSON parsing strategy, Python helper scripts, future pybind11/native binding.

---

### Task 1: Build Hygiene

**Files:**
- Modify: `CMakeLists.txt`
- Modify: `CMakePresets.json`
- Create: `.gitattributes`

- [x] Add a `MOYI_BUILD_TESTS` option.
- [x] Add CTest integration.
- [x] Normalize source line endings with `.gitattributes`.
- [ ] Verify all current targets compile.

### Task 2: Core Tests

**Files:**
- Create: `core/tests/test_types.cpp`
- Create: `core/tests/test_context_retriever.cpp`
- Create: `core/tests/test_safety_checker.cpp`
- Create: `core/tests/test_learning_memory.cpp`
- Create: `core/tests/test_translation_session.cpp`

- [x] Add a minimal test harness or integrate a small test dependency.
- [x] Cover language pair conversion.
- [x] Cover glossary retrieval.
- [x] Cover safety keyword and low-confidence behavior.
- [x] Cover learning card extraction.
- [x] Cover text-mode `TranslationSession`.

### Task 3: Asset Loading

**Files:**
- Create: `core/include/moyi/assets/glossary_loader.h`
- Create: `core/include/moyi/assets/safety_loader.h`
- Create: `core/include/moyi/assets/model_manifest_loader.h`
- Create: `core/src/assets/glossary_loader.cpp`
- Create: `core/src/assets/safety_loader.cpp`
- Create: `core/src/assets/model_manifest_loader.cpp`

- [x] Load glossary JSON into `ContextTerm`.
- [x] Load safety JSON into `SafetyRule`.
- [x] Load model manifest YAML-like fields into `ModelManifest`.
- [x] Return structured `RuntimeError` on malformed assets.

### Task 4: CLI Asset Flags

**Files:**
- Modify: `apps/cli/src/main.cpp`

- [x] Add `--pair`.
- [x] Add `--glossary`.
- [x] Add `--safety`.
- [x] Add `--manifest`.
- [x] Keep deterministic default behavior when no assets are passed.

### Task 5: Benchmark Surface

**Files:**
- Modify: `apps/cli/src/main.cpp`
- Modify: `tools/benchmark/benchmark_python_pipeline.py`
- Create: `docs/benchmarks/baseline-template.json`

- [x] Add `moyi bench` or `--bench-samples`.
- [x] Emit JSON benchmark summaries.
- [x] Record sample count, elapsed time, average time, and runtime adapter.

### Task 6: Native Python Binding Decision

**Files:**
- Create: `docs/architecture/python-native-binding.md`

- [x] Compare pybind11, C ABI + ctypes, and subprocess CLI bridge.
- [x] Choose one binding path for the next implementation pass.
- [x] Define package layout and ownership boundary.

### Task 7: ONNX Runtime Integration Plan

**Files:**
- Create: `docs/architecture/onnx-runtime-integration.md`

- [x] Define dependency strategy.
- [x] Define tokenizer ownership.
- [x] Define missing-model behavior.
- [x] Define first model selection criteria.

### Task 8: CI Plan

**Files:**
- Create: `.github/workflows/native-foundation.yml`

- [x] Add configure/build/test workflow.
- [x] Keep model artifacts out of CI.
- [x] Run deterministic mock-runtime tests only.

## Completion Criteria

The foundation hardening phase is complete when:

- CMake configure/build succeeds on a clean machine.
- CTest runs core tests.
- CLI can run default deterministic demo.
- CLI can load glossary and safety packs from files.
- Benchmark output is machine-readable.
- Python native binding and ONNX integration have clear implementation plans.
