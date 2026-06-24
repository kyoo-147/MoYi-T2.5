# Benchmark Plan

Benchmarks are intentionally not run in this implementation pass.

Future benchmark dimensions:

- End-to-end utterance latency.
- ASR latency by audio length.
- Translation latency by language pair.
- Memory usage per runtime adapter.
- Safety checker overhead.
- Learning card extraction overhead.

Target MVP budgets:

- Text path under 250 ms on desktop CPU with mock/local lightweight adapters.
- Local translation under 800 ms per short utterance for edge demo models.
- Total on-device memory budget under 512 MB for VN-EN translation path.
