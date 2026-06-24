# MoYi Next Phase Brief

## North Star

MoYi should become a portable edge translation runtime that can run across:

- Android phones
- laptop / edge box deployments
- wearable or embedded dev kits

The next phase must make the foundation trustworthy before adding product UI. The priority is not "make a pretty app"; it is to make the runtime buildable, testable, measurable, and ready to receive real local models.

## Current Foundation

The repository already contains:

- C++20 core contracts and pipeline skeleton
- mock ASR and mock translation adapters
- ONNX adapter boundary
- local glossary/context retrieval
- rule-based safety checker
- simple learning-memory extraction
- native CLI surface
- Python SDK mirror
- Android JNI placeholder boundary
- VN-EN factory sample assets

## Strategic Gap

The MVP scaffold is useful, but it is not yet a strong engineering foundation until these are true:

1. The C++ project builds consistently on a clean machine.
2. The CLI can run an end-to-end deterministic demo.
3. Core behavior is covered by tests.
4. Glossary and safety packs are loaded from asset files instead of hard-coded in CLI/Python.
5. Runtime adapters expose a stable model-manifest contract.
6. The ONNX adapter is wired to real inference for at least one small local model.
7. Python and Android call the native core instead of diverging as separate implementations.

## Product Positioning For Development

MoYi is not a generic translator. Development should keep these differentiators visible:

- context-aware workplace translation
- safety-first confirmation for critical commands
- local/edge execution path
- company glossary and SOP awareness
- learning memory generated from real conversations

Any feature that does not strengthen one of these should wait.

## Next Phase Goal

Turn the repo from "MVP scaffold" into "verified edge runtime alpha":

```text
clone repo
configure project
build native core + CLI
run deterministic vi-en factory demo
run unit tests
inspect benchmark output
swap mock adapter with ONNX adapter path when model assets are present
```

## Recommended Execution Order

1. Build hygiene and CI-ready structure.
2. Core unit tests and deterministic CLI demo.
3. Asset loading for glossary, safety rules, and model manifests.
4. Benchmark harness and latency/memory reporting.
5. Native Python binding to C++ core.
6. ONNX Runtime integration for one model path.
7. Android JNI wiring to real native session.
8. Streaming audio session and partial events.
9. VN-KR and VN-CN asset packs.

## Non-Goals For The Next Phase

- Full mobile app UI.
- Wearable hardware integration.
- Federated learning.
- Production enterprise admin portal.
- Multi-tenant backend.
- Fine-tuning workflows.

These can come later after the runtime alpha is stable.
