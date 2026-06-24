# ONNX Runtime Integration

MoYi keeps model execution behind runtime adapters. The ONNX adapter now has an optional real ONNX Runtime load path.

## Configure

```powershell
cmake -S . -B build/onnx `
  -DMOYI_ENABLE_REAL_ONNX=ON `
  -DMOYI_ONNXRUNTIME_ROOT=C:\path\to\onnxruntime
```

`MOYI_ONNXRUNTIME_ROOT` should contain:

```text
include/
lib/
```

## Current Capability

When enabled, the ONNX adapters:

- validate model path existence
- create `Ort::Env`
- create `Ort::SessionOptions`
- load an `Ort::Session`
- return precise errors for tokenizer/tensor I/O that is not wired yet

This is intentional. Model loading is separated from model-specific preprocessing and decoding.

## First Model Criteria

The first real model should be:

- small enough for CPU edge inference
- exportable to ONNX
- compatible with a tokenizer that can be loaded locally
- usable for short workplace utterances
- benchmarkable under the `latency_budget_ms` and `memory_budget_mb` in the manifest

## Missing-Model Behavior

If model artifacts are absent, the adapter must fail with `onnx_model_not_found`. It must not silently fall back to cloud APIs.
