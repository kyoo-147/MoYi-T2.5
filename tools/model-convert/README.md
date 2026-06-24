# Model Conversion Notes

This folder is reserved for repeatable conversion scripts from research models to edge-friendly artifacts.

Initial target artifacts:

- ONNX int8 translation model for `vi-en`.
- ONNX or platform-native ASR model for Vietnamese.
- Tokenizer files stored beside the model manifest.

Future target runtimes:

- TFLite for Android CPU/NNAPI.
- QNN for Qualcomm devices.
- llama.cpp/ggml-style quantized models for compact local inference.
