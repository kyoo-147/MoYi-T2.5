from __future__ import annotations

from dataclasses import dataclass, field


@dataclass(frozen=True)
class LanguagePair:
    source: str = "vi"
    target: str = "en"

    @property
    def code(self) -> str:
        return f"{self.source}-{self.target}"


@dataclass(frozen=True)
class Confidence:
    asr: float
    translation: float
    overall: float


@dataclass(frozen=True)
class ContextTerm:
    source: str
    target: str
    domain: str = "general"


@dataclass(frozen=True)
class RetrievedContext:
    profile_id: str
    role: str
    glossary_terms: list[ContextTerm] = field(default_factory=list)
    safety_terms: list[str] = field(default_factory=list)


@dataclass(frozen=True)
class SafetyDecision:
    flagged: bool
    severity: str
    requires_confirmation: bool
    reason: str = ""


@dataclass(frozen=True)
class LearningCard:
    front: str
    back: str
    type: str
    source_utterance: str


@dataclass(frozen=True)
class TranslationResult:
    session_id: str
    language_pair: LanguagePair
    source_text: str
    translated_text: str
    confidence: Confidence
    safety: SafetyDecision
    context_used: RetrievedContext
    learning_cards: list[LearningCard]
