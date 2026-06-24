from __future__ import annotations

import argparse
import json
from dataclasses import asdict

from .pipeline import MoYiPipeline
from .native import NativeMoYiPipeline


def main() -> None:
    parser = argparse.ArgumentParser(description="MoYi Edge Python SDK demo")
    parser.add_argument("--text", default="Dung may lai, kiem tra cam bien an toan.")
    parser.add_argument("--profile", default="factory_worker")
    parser.add_argument("--native", action="store_true", help="Use the native C++ runtime through the C ABI bridge")
    args = parser.parse_args()

    if args.native:
        with NativeMoYiPipeline() as pipeline:
            print(json.dumps(pipeline.process_text(args.text), ensure_ascii=False, indent=2))
        return

    pipeline = MoYiPipeline(profile_id=args.profile)
    result = pipeline.process_text(args.text)
    payload = asdict(result)
    payload["language_pair"] = result.language_pair.code
    print(json.dumps(payload, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
