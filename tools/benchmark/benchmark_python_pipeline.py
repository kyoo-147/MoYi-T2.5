from __future__ import annotations

import time

from moyi_edge import MoYiPipeline


def main() -> None:
    pipeline = MoYiPipeline()
    samples = [
        "Dung may lai, kiem tra cam bien an toan.",
        "Khi nao may nay can bao tri?",
        "Day chuyen so hai dang chay cham.",
    ]

    started = time.perf_counter()
    for sample in samples:
        pipeline.process_text(sample)
    elapsed_ms = (time.perf_counter() - started) * 1000
    print(
        {
            "samples": len(samples),
            "elapsed_ms": round(elapsed_ms, 3),
            "avg_ms": round(elapsed_ms / len(samples), 3),
        }
    )


if __name__ == "__main__":
    main()
