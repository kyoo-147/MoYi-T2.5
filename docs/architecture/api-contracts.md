# MoYi API Contracts

## TranslationResult

All clients should preserve this shape whether the call comes from C++, Python, Android, or a future web/app shell.

```json
{
  "session_id": "cli-demo",
  "language_pair": "vi-en",
  "source_text": "Dung may lai, kiem tra cam bien an toan.",
  "translated_text": "Stop the machine and check the safety sensor.",
  "confidence": {
    "asr": 1.0,
    "translation": 0.88,
    "overall": 0.94
  },
  "safety": {
    "flagged": true,
    "severity": "critical",
    "requires_confirmation": true,
    "reason": "Contains stop-machine safety command"
  },
  "context_used": {
    "profile_id": "factory_worker",
    "role": "factory_worker",
    "glossary_terms": [
      {
        "source": "cam bien an toan",
        "target": "safety sensor",
        "domain": "machine_safety"
      }
    ],
    "safety_terms": ["dung may"]
  },
  "learning_cards": [
    {
      "front": "cam bien an toan",
      "back": "safety sensor",
      "type": "term",
      "source_utterance": "Dung may lai, kiem tra cam bien an toan."
    }
  ]
}
```

## Session Events

Realtime clients should subscribe to these event names:

- `AudioChunkReceived`
- `SpeechDetected`
- `PartialTranscriptReady`
- `FinalTranscriptReady`
- `ContextRetrieved`
- `PartialTranslationReady`
- `FinalTranslationReady`
- `SafetyWarningRaised`
- `OutputReady`
- `LearningCardsGenerated`
- `ErrorRaised`

## Runtime Adapter Rule

Pipeline code calls only core interfaces. ONNX, TFLite, QNN, llama.cpp, cloud fallback, and mocks must remain behind adapter classes.
