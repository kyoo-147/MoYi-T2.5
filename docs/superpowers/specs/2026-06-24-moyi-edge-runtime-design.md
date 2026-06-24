# MoYi Edge Runtime MVP Design

## Purpose

MoYi is an edge-first translation runtime for multilingual workplace communication. The MVP prioritizes a reusable C++ core that can later be optimized for Android phones, laptop/edge boxes, and wearable/dev-kit deployments. Web and app layers are consumers of the runtime, not the center of the architecture.

## Approved Decisions

- Repository: `github.com/kyoo-147/moyi-edge-translation`.
- Targets: Android phone, laptop/edge box, wearable/dev kit.
- MVP strategy: hybrid edge-first plus runtime skeleton.
- Core: C++ engine first, Python CLI/SDK for demos and benchmarks.
- Language design: multi-language architecture, VN-EN demo first.
- Runtime design: adapter-neutral, mock and ONNX first, room for TFLite, llama.cpp/ggml, and QNN.
- Pipeline design: sync API for tests/CLI and streaming session events for realtime clients.
- Acceptance surface: SDK/library is the product, CLI is the demo.

## Architecture

The runtime is split into small units:

- Core contracts and pipeline orchestration.
- Runtime adapters for ASR and translation engines.
- Local retrieval for glossary/profile/safety context.
- Rule-based safety checker for MVP critical commands.
- Learning memory extractor for vocabulary and phrase cards.
- CLI and Python bindings as thin clients.

The core never calls ONNX Runtime, Python, Android, or web APIs directly. It depends only on interfaces such as `IASRAdapter`, `ITranslationAdapter`, `IContextRetriever`, `ISafetyChecker`, and `ILearningMemory`.

## MVP Scope

The MVP code foundation includes:

- C++ core library.
- Mock adapters for deterministic development.
- ONNX adapter skeleton for future model-backed inference.
- CLI that can run a text pipeline through context, translation, safety, and learning memory.
- Python SDK mirror for scripting and benchmarking.
- Assets for VN-EN glossary, safety rules, sample text, and model manifest.
- Android JNI skeleton showing the future mobile binding boundary.

Build, test, and runtime verification are intentionally deferred per user request.
