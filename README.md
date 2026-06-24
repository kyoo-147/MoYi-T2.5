# MoYi Edge Translation

**MoYi - Personalized Edge AI Translation Companion** is an edge-first runtime foundation for real-time, context-aware workplace translation.

The project targets multilingual work environments such as factories, logistics operations, construction sites, hospitals, and FDI businesses, where Vietnamese workers need fast and safe communication with foreign managers, colleagues, or clients.

MoYi is designed as a portable **edge AI translation runtime first**. Web, mobile, and wearable apps are treated as clients on top of the same core engine.

Repository: `github.com/kyoo-147/moyi-edge-translation`

## Product Vision

Conventional translation apps translate isolated sentences. MoYi is built around workplace context:

- worker role and language level
- company glossary and internal abbreviations
- SOP phrases and safety commands
- conversation history and learning memory
- safety-first checks for critical instructions

After each conversation, MoYi can turn real work communication into vocabulary, phrase cards, and reusable learning memory.

## MVP Focus

The current MVP codebase focuses on the runtime foundation:

- C++20 core engine
- adapter-neutral model runtime interfaces
- mock runtime for deterministic local development
- ONNX runtime boundary for future local model inference
- Python SDK mirror for scripting and benchmarks
- native C ABI bridge for Python and other clients
- native CLI acceptance demo
- Android JNI session bridge for future mobile integration
- VN-EN, VN-KR, and VN-CN sample assets for factory/workplace scenarios

The first demo language pair is **Vietnamese-English (`vi-en`)**, while the architecture is designed for future VN-KR and VN-CN support.

## Architecture

```text
Apps / SDKs
  CLI
  Python SDK
  Android JNI
  future mobile/web/wearable clients

Core Engine
  TranslationSession
  ContextRetriever
  SafetyChecker
  LearningMemory
  ASR / Translation interfaces

Runtime Adapters
  Mock
  ONNX
  future TFLite
  future llama.cpp / ggml
  future QNN

Assets
  Model manifests
  Glossary packs
  Safety rules
  Sample utterances
```

Core processing pipeline:

```text
Audio/Text Input
  -> Audio Frontend
  -> ASR Adapter
  -> Language Pair Resolver
  -> Context Retriever
  -> Translation Adapter
  -> Safety Checker
  -> Output Composer
  -> Learning Memory Extractor
```

## Repository Layout

```text
core/                 C++ runtime contracts and pipeline orchestration
runtimes/mock/        Deterministic local ASR/translation adapters
runtimes/onnx/        ONNX adapter boundary for future local inference
bindings/python/      Python SDK mirror and CLI helper
bindings/android-jni/ Android binding skeleton
apps/cli/             Native CLI acceptance demo
apps/examples/        Example usage
assets/               Model manifests, glossary packs, safety rules, samples
docs/                 Architecture docs, specs, plans, benchmark notes
tools/                Model conversion and benchmark helper scripts
```

## Current Status

This repository is an MVP foundation/prototype. It includes real runtime contracts, local deterministic adapters, native bridge surfaces, asset loaders, and an optional ONNX Runtime model-loading path. Production tokenizer/tensor I/O for real translation models is still a roadmap item.

Implemented:

- C++ core contracts and orchestration
- CMake/CTest build structure
- local context retrieval
- C++ asset loaders for glossary, safety rules, and model manifests
- rule-based safety checker
- simple learning card extraction
- mock ASR and translation adapters
- ONNX Runtime optional model-loading path
- Python SDK mirror
- Python native mode through `moyi_c_api`
- CLI demo surface
- CLI benchmark JSON output
- Android JNI native session boundary
- sample VN-EN, VN-KR, and VN-CN factory glossary/safety/model packs

Not yet implemented:

- ONNX tokenizer and tensor I/O for production inference
- model conversion scripts for real artifacts
- full microphone streaming integration
- native Android app UI
- wearable-specific deployment package
- full production QA across all target devices

## Build Skeleton

The project uses CMake.

```powershell
cmake --preset default
cmake --build --preset default
ctest --test-dir build/default --output-on-failure
```

The native CLI target is named:

```text
moyi
```

Example intended usage:

```powershell
moyi --text "Dung may lai, kiem tra cam bien an toan." --pair vi-en
```

Asset-driven demo:

```powershell
moyi --text "Dung may lai, kiem tra cam bien an toan." `
  --pair vi-en `
  --glossary assets/glossary/factory_vi_en.json `
  --safety assets/safety/factory_rules_vi_en.json `
  --manifest assets/models/vi-en/manifest.yaml
```

Benchmark JSON output:

```powershell
moyi --bench-samples assets/samples/factory_utterances_vi.txt
```

## Python SDK Skeleton

The Python helper package lives in `bindings/python`.

Example intended usage:

```powershell
cd bindings/python
python -m moyi_edge.cli --text "Dung may lai, kiem tra cam bien an toan."
```

Native C++ runtime through the C ABI bridge:

```powershell
python -m moyi_edge.cli --native --text "Dung may lai, kiem tra cam bien an toan."
```

If the native library is not discovered under `build/**`, set `MOYI_C_API_LIBRARY`.

Python output follows the same result contract as the C++ runtime:

- `session_id`
- `language_pair`
- `source_text`
- `translated_text`
- confidence scores
- safety decision
- context used
- learning cards

## Sample Assets

Included MVP assets:

- `assets/models/vi-en/manifest.yaml`
- `assets/models/vi-ko/manifest.yaml`
- `assets/models/vi-zh/manifest.yaml`
- `assets/glossary/factory_vi_en.json`
- `assets/glossary/factory_vi_ko.json`
- `assets/glossary/factory_vi_zh.json`
- `assets/safety/factory_rules_vi_en.json`
- `assets/safety/factory_rules_vi_ko.json`
- `assets/safety/factory_rules_vi_zh.json`
- `assets/samples/factory_utterances_vi.txt`

Large model artifacts are intentionally not committed. The manifest documents the expected path and deployment budget for future ONNX assets.

## Edge Target Strategy

MoYi is designed to support three deployment classes from the same core:

- **Android phone:** C++ core through JNI, optimized local models over time.
- **Laptop / edge box:** native CLI or Python SDK for booth demos, pilots, and benchmarks.
- **Wearable / dev kit:** compact session/event contract behind device-specific audio and UI layers.

## Safety-First Translation

The MVP safety layer uses deterministic local rules. A phrase such as:

```text
Dung may lai, kiem tra cam bien an toan.
```

can be flagged as a critical safety command and marked as requiring confirmation before output is trusted.

Future versions can add classifier-based safety checks while keeping the same core interface.

## Roadmap

1. Wire real ONNX Runtime inference for one VN-EN translation model.
2. Add tokenizer and tensor I/O for ONNX translation and ASR models.
3. Add local ASR model support for Vietnamese.
4. Add streaming microphone input and partial events.
5. Add Android demo app using the JNI boundary.
6. Explore TFLite, QNN, and llama.cpp/ggml runtime adapters.

Detailed planning docs:

- `docs/NEXT_PHASE_BRIEF.md`
- `docs/TECHNICAL_ROADMAP.md`
- `docs/superpowers/plans/2026-06-24-next-foundation-hardening.md`

## License

No license has been selected yet.
