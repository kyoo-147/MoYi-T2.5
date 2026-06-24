# MoYi Edge Runtime MVP Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the MVP code foundation for `github.com/kyoo-147/moyi-edge-translation`: an edge-first translation runtime with C++ core, adapter-neutral model interfaces, mock/ONNX runtime stubs, Python SDK, and CLI acceptance demo.

**Architecture:** The core is a C++ library exposing stable contracts for audio, ASR, translation, retrieval, safety, learning memory, and session orchestration. Runtime-specific code lives behind adapters, with mock and ONNX adapters first; Python and CLI are consumers rather than owners of the pipeline.

**Tech Stack:** C++20, CMake, optional ONNX Runtime adapter boundary, Python 3 SDK/CLI helpers, JSON/YAML-like asset manifests.

---

### Task 1: Repo Foundation

**Files:**
- Create: `README.md`
- Create: `CMakeLists.txt`
- Create: `.gitignore`
- Create: `docs/superpowers/specs/2026-06-24-moyi-edge-runtime-design.md`

- [x] Add project description, build layout, and MVP scope.
- [x] Add top-level CMake targets for core, mock runtime, ONNX runtime stub, and CLI.
- [x] Document edge-first design decisions.

### Task 2: C++ Core Contracts

**Files:**
- Create: `core/include/moyi/types.h`
- Create: `core/include/moyi/result.h`
- Create: `core/include/moyi/models/model_manifest.h`
- Create: `core/include/moyi/audio/audio_frontend.h`
- Create: `core/include/moyi/translation/asr_adapter.h`
- Create: `core/include/moyi/translation/translation_adapter.h`
- Create: `core/include/moyi/context/context_retriever.h`
- Create: `core/include/moyi/safety/safety_checker.h`
- Create: `core/include/moyi/learning/learning_memory.h`
- Create: `core/include/moyi/pipeline/session.h`

- [x] Define language pairs, confidence, transcript, translation, safety, and learning-card contracts.
- [x] Define interfaces so model runtimes are replaceable.
- [x] Define sync and streaming session APIs.

### Task 3: Core Implementations

**Files:**
- Create implementation files under `core/src/**`.

- [x] Implement default context retriever.
- [x] Implement rule-based safety checker.
- [x] Implement simple learning memory extractor.
- [x] Implement translation session orchestration.

### Task 4: Runtime Adapters

**Files:**
- Create: `runtimes/mock/**`
- Create: `runtimes/onnx/**`

- [x] Add mock ASR and translation adapters for deterministic local demos.
- [x] Add ONNX adapter boundary and model manifest loading surface.

### Task 5: CLI and Python SDK

**Files:**
- Create: `apps/cli/src/main.cpp`
- Create: `bindings/python/moyi_edge/**`

- [x] Add C++ CLI demo surface for text translation.
- [x] Add Python dataclasses and SDK pipeline mirror for scripting and benchmarks.

### Task 6: Assets and Target Skeletons

**Files:**
- Create: `assets/**`
- Create: `bindings/android-jni/**`
- Create: `tools/**`
- Create: `docs/architecture/**`

- [x] Add sample glossary, safety rules, model manifest, and input text.
- [x] Add Android JNI placeholder boundary.
- [x] Add benchmark/model conversion placeholders as real entry scripts.

### Note On Verification

The user explicitly requested implementation first with no test/check/run. This plan records code completion steps only; build/test execution is intentionally deferred.
