# MoYi Edge Translation

> **Public-source note**
>
> This public repository does not always reflect the newest private implementation of MoYi. Some source code, model work, demos, competition materials, and commercial details are intentionally not published yet because of competition, business, and partnership constraints.

![MoYi Edge Translation Runtime](docs/images/moyi-banner.png)

**MoYi - Personalized Edge AI Translation Companion** is an edge-first translation runtime for real-time, context-aware workplace communication.

MoYi is built for environments where translation is not just language conversion: factories, logistics operations, construction sites, hospitals, field-service teams, FDI companies, and remote teams that need fast multilingual coordination with safety and domain context.

The project is designed as a portable **edge translation runtime first**. Desktop tools, Python workflows, mobile apps, and embedded deployments are clients on top of the same core engine.

Repository: `github.com/kyoo-147/moyi-edge-translation`

## Why MoYi Exists

Generic translation apps usually translate isolated sentences. That is not enough in real workplaces.

In a factory, logistics site, or cross-border remote team, a translation system needs to understand:

- worker roles and communication level
- company glossary and internal abbreviations
- SOP phrases, safety commands, and domain-specific words
- conversation context from the current task
- phrases that must be confirmed before action
- what should become vocabulary, flashcards, or reusable learning memory

MoYi focuses on this product wedge: **private, context-aware translation for operational work**, where mistranslation can slow down work, create rework, or increase safety risk.

## Current Product Direction

MoYi is moving from runtime foundation toward field-test and pitch readiness.

The current private direction includes:

- local-first translation runtime for workplace and team communication
- C++20 core that can be embedded into desktop, Python tooling, and mobile clients
- adapter-ready inference boundary for ONNX Runtime and llama.cpp
- workflow-aware translation using glossary, safety policy, and context retrieval
- realtime meeting/workflow usage for internal remote collaboration
- deployment evaluation across desktop, Android ARM, Raspberry Pi, embedded Linux, Qualcomm devices, and Intel edge devices

This public repository contains the core foundation and a deterministic development path, while the newest competition/commercial implementation may remain private.

## Visual Overview

### User Flow

![MoYi user flow](docs/images/moyi-user-flow.png)

MoYi is designed around an operational loop:

1. Capture meeting, chat, or workplace utterance.
2. Detect language pair and task context.
3. Retrieve glossary, SOP terms, safety rules, and recent conversation context.
4. Run translation through a runtime adapter.
5. Validate safety-sensitive phrases.
6. Return translated output with confidence and confirmation signals.
7. Extract reusable learning memory.

### System Architecture

![MoYi system architecture](docs/images/moyi-system-architecture.png)

The public runtime is split into four layers:

- **Client surfaces:** CLI, Python SDK, native bridge, mobile/JNI boundary, and future app surfaces.
- **Core orchestration:** `TranslationSession`, language pair resolution, context retrieval, safety checking, output composition, and learning memory extraction.
- **Runtime adapters:** deterministic mock adapter for development, ONNX Runtime boundary, and future runtime targets such as llama.cpp, TFLite, ExecuTorch, and QNN/NNAPI-backed paths.
- **Local assets:** model manifests, glossary packs, safety rules, sample utterances, and future local model artifacts.

### Processing Pipeline

![MoYi processing pipeline](docs/images/moyi-processing-pipeline.png)

The translation pipeline is intentionally explicit so that each stage can be tested and benchmarked:

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

### Backend Runtime Boundary

![MoYi backend architecture](docs/images/moyi-backend-architecture.png)

The backend/runtime design keeps model execution behind adapter contracts. This lets the project develop and test the pipeline with deterministic adapters while keeping a path toward real local inference runtimes.

The current public repository includes:

- deterministic local mock ASR and translation adapters for development and tests
- ONNX Runtime adapter boundary and optional model-loading path
- C ABI bridge for native integration
- Python SDK mirror and native mode
- Android JNI session boundary
- asset loaders for glossary, safety rules, and model manifests

The adapter boundary is meant to prevent product logic from being locked to one model provider or one deployment device.

## Edge Deployment Matrix

![MoYi edge deployment matrix](docs/images/moyi-deployment-matrix.png)

| Device group | Runtime candidates | Evaluation focus |
| --- | --- | --- |
| Desktop x86 | ONNX Runtime, llama.cpp | Latency, SIMD, memory |
| Android ARM | ONNX Runtime Mobile, TFLite, ExecuTorch | NNAPI, binary size, battery |
| Raspberry Pi | TFLite, ONNX Runtime, llama.cpp | ARM NEON, thermal, RAM |
| Embedded Linux | ONNX Runtime, ExecuTorch | Cross-compilation, memory |
| Qualcomm devices | QNN/NNAPI-backed runtime | NPU delegation |
| Intel Edge | OpenVINO or ONNX Runtime | INT8, CPU/NPU acceleration |

ExecuTorch uses an export, compile/quantize/partition flow and runs models through a lightweight C++ runtime on device. ONNX Runtime Mobile also supports reducing model and runtime size for mobile deployment.

## Data And Asset Model

![MoYi data model](docs/images/moyi-data-model.png)

MoYi keeps operational knowledge outside the core engine where possible:

- model manifests describe language pairs and deployment budgets
- glossary packs define company/domain-specific vocabulary
- safety packs define critical phrases and confirmation requirements
- sample utterances support deterministic testing and benchmarks
- learning cards can be produced from translated work conversations

Current sample packs cover:

- Vietnamese-English (`vi-en`)
- Vietnamese-Korean (`vi-ko`)
- Vietnamese-Chinese (`vi-zh`)

The first public demo pair is **Vietnamese-English (`vi-en`)**.

## Repository Layout

```text
core/                 C++ runtime contracts and pipeline orchestration
runtimes/mock/        Deterministic local ASR/translation adapters
runtimes/onnx/        ONNX adapter boundary for local inference
bindings/python/      Python SDK mirror and CLI helper
bindings/native/      C ABI bridge
bindings/android-jni/ Android JNI session boundary
apps/cli/             Native CLI acceptance demo
apps/examples/        Example usage
assets/               Model manifests, glossary packs, safety rules, samples
docs/                 Architecture docs, specs, plans, benchmark notes
tools/                Model conversion and benchmark helper scripts
```

## Current Implementation Status

This repository is a public runtime foundation, not the complete private product.

Implemented in the public repo:

- C++20 core contracts and orchestration
- CMake/CTest build structure
- `TranslationSession` orchestration
- local context retrieval
- glossary, safety-rule, and model-manifest loaders
- rule-based safety checker
- simple learning-card extraction
- deterministic mock ASR and translation adapters
- optional ONNX Runtime model-loading path
- Python SDK mirror
- Python native mode through `moyi_c_api`
- CLI demo surface
- CLI benchmark JSON output
- Android JNI native session boundary
- sample VN-EN, VN-KR, and VN-CN factory glossary/safety/model packs

Not fully public or not complete yet:

- newest private runtime and product experiments
- competition and commercial demo materials
- production tokenizer and tensor I/O for real translation models
- full realtime microphone streaming
- full mobile app UI
- field-test telemetry and pitch/funding materials
- production QA across all target devices

## Build And Test

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

Example text demo:

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

## Python SDK

The Python helper package lives in `bindings/python`.

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

## Safety-First Translation

The public MVP safety layer uses deterministic local rules. A phrase such as:

```text
Dung may lai, kiem tra cam bien an toan.
```

can be flagged as a critical safety command and marked as requiring confirmation before output is trusted.

The product direction is to keep safety checks local and composable, so later classifier-based safety checks can be added without changing the public session contract.

## Business And Impact Framing

MoYi is not intended to compete only as a generic translator. The value is in operational context:

- fewer missed instructions in multilingual work
- lower communication friction for Vietnamese workers and foreign managers
- safer handling of critical commands
- reusable company vocabulary and training memory
- lower dependency on always-online cloud translation in privacy-sensitive settings
- measurable latency, glossary accuracy, safety recall, and learning-card quality

The project is being positioned toward practical trials, pitching, and funding conversations, while keeping sensitive or competition-bound implementation details private.

## Roadmap

Near-term engineering path:

1. Harden deterministic build/test/benchmark flow.
2. Wire real ONNX Runtime inference for one VN-EN model path.
3. Add tokenizer and tensor I/O for ONNX translation and ASR models.
4. Add local ASR model support for Vietnamese.
5. Add streaming microphone input and partial events.
6. Expand mobile runtime integration through the JNI boundary.
7. Evaluate ONNX Runtime Mobile, TFLite, ExecuTorch, llama.cpp, QNN/NNAPI, and OpenVINO paths.
8. Prepare field-test metrics for latency, memory, glossary accuracy, safety recall, and user workflow impact.

Detailed planning docs:

- `docs/NEXT_PHASE_BRIEF.md`
- `docs/TECHNICAL_ROADMAP.md`
- `docs/superpowers/plans/2026-06-24-next-foundation-hardening.md`

## License

No license has been selected yet.
