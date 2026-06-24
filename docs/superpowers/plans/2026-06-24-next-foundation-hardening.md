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

- [ ] Add a `MOYI_BUILD_TESTS` option.
- [ ] Add CTest integration.
- [ ] Normalize source line endings with `.gitattributes`.
- [ ] Verify all current targets compile.

### Task 2: Core Tests

**Files:**
- Create: `core/tests/test_types.cpp`
- Create: `core/tests/test_context_retriever.cpp`
- Create: `core/tests/test_safety_checker.cpp`
- Create: `core/tests/test_learning_memory.cpp`
- Create: `core/tests/test_translation_session.cpp`

- [ ] Add a minimal test harness or integrate a small test dependency.
- [ ] Cover language pair conversion.
- [ ] Cover glossary retrieval.
- [ ] Cover safety keyword and low-confidence behavior.
- [ ] Cover learning card extraction.
- [ ] Cover text-mode `TranslationSession`.

### Task 3: Asset Loading

**Files:**
- Create: `core/include/moyi/assets/glossary_loader.h`
- Create: `core/include/moyi/assets/safety_loader.h`
- Create: `core/include/moyi/assets/model_manifest_loader.h`
- Create: `core/src/assets/glossary_loader.cpp`
- Create: `core/src/assets/safety_loader.cpp`
- Create: `core/src/assets/model_manifest_loader.cpp`

- [ ] Load glossary JSON into `ContextTerm`.
- [ ] Load safety JSON into `SafetyRule`.
- [ ] Load model manifest YAML-like fields into `ModelManifest`.
- [ ] Return structured `RuntimeError` on malformed assets.

### Task 4: CLI Asset Flags

**Files:**
- Modify: `apps/cli/src/main.cpp`

- [ ] Add `--pair`.
- [ ] Add `--glossary`.
- [ ] Add `--safety`.
- [ ] Add `--manifest`.
- [ ] Keep deterministic default behavior when no assets are passed.

### Task 5: Benchmark Surface

**Files:**
- Modify: `apps/cli/src/main.cpp`
- Modify: `tools/benchmark/benchmark_python_pipeline.py`
- Create: `docs/benchmarks/baseline-template.json`

- [ ] Add `moyi bench` or `--bench-samples`.
- [ ] Emit JSON benchmark summaries.
- [ ] Record sample count, elapsed time, average time, and runtime adapter.

### Task 6: Native Python Binding Decision

**Files:**
- Create: `docs/architecture/python-native-binding.md`

- [ ] Compare pybind11, C ABI + ctypes, and subprocess CLI bridge.
- [ ] Choose one binding path for the next implementation pass.
- [ ] Define package layout and ownership boundary.

### Task 7: ONNX Runtime Integration Plan

**Files:**
- Create: `docs/architecture/onnx-runtime-integration.md`

- [ ] Define dependency strategy.
- [ ] Define tokenizer ownership.
- [ ] Define missing-model behavior.
- [ ] Define first model selection criteria.

### Task 8: CI Plan

**Files:**
- Create: `.github/workflows/native-foundation.yml`

- [ ] Add configure/build/test workflow.
- [ ] Keep model artifacts out of CI.
- [ ] Run deterministic mock-runtime tests only.

## Completion Criteria

The foundation hardening phase is complete when:

- CMake configure/build succeeds on a clean machine.
- CTest runs core tests.
- CLI can run default deterministic demo.
- CLI can load glossary and safety packs from files.
- Benchmark output is machine-readable.
- Python native binding and ONNX integration have clear implementation plans.
