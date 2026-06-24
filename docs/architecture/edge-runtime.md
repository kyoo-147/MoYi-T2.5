# MoYi Edge Runtime Architecture

## Core Idea

MoYi is a portable edge translation runtime. The C++ core owns the pipeline and data contracts; model runtimes, Python, Android, and future web/app layers connect through adapters.

## Runtime Layers

```text
Apps / SDKs
  CLI
  Python SDK
  Android JNI
  future Web/App shells

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

## MVP Acceptance Shape

The first useful demo is a CLI/SDK run that accepts text or audio, retrieves local glossary and safety context, translates through a replaceable adapter, flags critical safety commands, and emits learning cards.

## Device Target Strategy

- Android phone: use the same C++ core through JNI.
- Laptop/edge box: use C++ CLI or Python SDK for deployment and benchmarks.
- Wearable/dev kit: keep the session/event contract small enough to embed behind device-specific audio and UI layers.
