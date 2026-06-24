from __future__ import annotations

import json
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class GlossaryPack:
    id: str
    language_pair: str
    industry: str
    terms: list[dict[str, str]]


@dataclass(frozen=True)
class SafetyRulePack:
    id: str
    language_pair: str
    rules: list[dict[str, object]]


def load_glossary(path: str | Path) -> GlossaryPack:
    payload = json.loads(Path(path).read_text(encoding="utf-8"))
    return GlossaryPack(
        id=payload["id"],
        language_pair=payload["language_pair"],
        industry=payload["industry"],
        terms=list(payload["terms"]),
    )


def load_safety_rules(path: str | Path) -> SafetyRulePack:
    payload = json.loads(Path(path).read_text(encoding="utf-8"))
    return SafetyRulePack(
        id=payload["id"],
        language_pair=payload["language_pair"],
        rules=list(payload["rules"]),
    )
