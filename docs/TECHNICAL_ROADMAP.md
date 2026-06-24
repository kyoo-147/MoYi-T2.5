# MoYi Technical Roadmap

## Phase 0 - Scaffold Complete

Status: done.

Delivered:

- C++ core contracts
- mock runtime
- ONNX boundary
- Python SDK mirror
- native CLI skeleton
- Android JNI placeholder
- sample VN-EN assets
- architecture docs

## Phase 1 - Verified Native Foundation

Goal: make the current C++ foundation buildable, testable, and deterministic.

Deliverables:

- CMake builds `moyi_core`, `moyi_runtime_mock`, `moyi_runtime_onnx`, and `moyi`.
- CLI runs text demo with hard-coded sample packs.
- Unit tests cover:
  - language code conversion
  - context retrieval
  - safety checker
  - learning card generation
  - translation session orchestration
- GitHub Actions or equivalent CI runs configure/build/test.
- README build instructions are verified.

Acceptance:

```text
cmake --preset default
cmake --build --preset default
ctest --test-dir build/default
build/default/moyi --text "Dung may lai, kiem tra cam bien an toan."
```

## Phase 2 - Asset-Driven Runtime

Goal: move from hard-coded context to file-driven packs.

Deliverables:

- C++ asset loader for glossary JSON.
- C++ asset loader for safety-rule JSON.
- C++ model manifest loader.
- CLI accepts:
  - `--glossary assets/glossary/factory_vi_en.json`
  - `--safety assets/safety/factory_rules_vi_en.json`
  - `--manifest assets/models/vi-en/manifest.yaml`
- Python SDK can load the same assets.
- Asset schema docs are versioned.

Acceptance:

```text
moyi --text "Dung may lai" --pair vi-en --glossary ... --safety ...
```

The output must prove context and safety were loaded from files.

## Phase 3 - Benchmark And Runtime Quality

Goal: measure runtime behavior before adding heavier models.

Deliverables:

- C++ benchmark CLI mode.
- Python benchmark script calls the native CLI or native binding.
- JSON benchmark reports for:
  - text path latency
  - safety checker overhead
  - context retrieval overhead
  - learning-card extraction overhead
- Baseline report committed under `docs/benchmarks`.

Acceptance:

```text
moyi bench --samples assets/samples/factory_utterances_vi.txt --out docs/benchmarks/baseline.json
```

## Phase 4 - Native Python Binding

Goal: stop Python from being a separate implementation.

Deliverables:

- Use `pybind11` or a small C ABI wrapper.
- `moyi_edge` package calls the native C++ core.
- Python fallback mirror remains only for development fallback if needed.
- Python result shape matches C++ `TranslationResult`.

Acceptance:

```text
python -m moyi_edge.cli --text "Dung may lai..."
```

This must execute the native session.

## Phase 5 - Real Local Model Path

Goal: prove edge AI path with at least one real local model component.

Deliverables:

- ONNX Runtime dependency option documented.
- `OnnxTranslationAdapter` loads model and tokenizer.
- First target can be a small text translation model or a controlled local model fixture.
- Clear fallback when model assets are missing.
- Model artifact download or conversion instructions.

Acceptance:

```text
moyi --runtime onnx --manifest assets/models/vi-en/manifest.yaml --text "..."
```

If model assets exist, inference runs locally. If not, CLI exits with a precise actionable error.

## Phase 6 - Android Native Session

Goal: Android binding uses the same native runtime.

Deliverables:

- JNI wrapper owns a real `TranslationSession`.
- Android-facing API returns the same JSON result contract.
- CMake/Gradle notes for Android build.
- Small Kotlin interface sample.

Acceptance:

```kotlin
MoYiNative.createSession()
MoYiNative.processText("Dung may lai...")
```

The native result should match CLI behavior.

## Phase 7 - Streaming Audio Runtime

Goal: make the architecture truly realtime.

Deliverables:

- Audio chunk queue.
- streaming session state machine.
- partial transcript event support.
- event callback examples.
- microphone sample for desktop or Android.

Acceptance:

Streaming mode must emit:

- `AudioChunkReceived`
- `PartialTranscriptReady`
- `FinalTranscriptReady`
- `SafetyWarningRaised`
- `OutputReady`

## Phase 8 - Multi-Language Expansion

Goal: prove architecture can expand beyond VN-EN.

Deliverables:

- VN-KR glossary and safety packs.
- VN-CN glossary and safety packs.
- language-pair resolver tests.
- model manifest examples for each pair.

Acceptance:

CLI can select `vi-en`, `vi-ko`, and `vi-zh` packs without code changes.
