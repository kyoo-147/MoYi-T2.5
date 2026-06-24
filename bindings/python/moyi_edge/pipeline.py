from __future__ import annotations

from .types import (
    Confidence,
    ContextTerm,
    LanguagePair,
    LearningCard,
    RetrievedContext,
    SafetyDecision,
    TranslationResult,
)


class MoYiPipeline:
    """Pure-Python MVP mirror of the C++ runtime contract.

    This intentionally stays small: it lets scripts and benchmarks use the
    same result shape before native bindings are wired.
    """

    def __init__(self, session_id: str = "python-demo", profile_id: str = "factory_worker") -> None:
        self.session_id = session_id
        self.profile_id = profile_id
        self.language_pair = LanguagePair("vi", "en")
        self.glossary = [
            ContextTerm("cam bien an toan", "safety sensor", "machine_safety"),
            ContextTerm("dung may", "stop the machine", "machine_safety"),
        ]
        self.safety_terms = ["dung may", "stop the machine"]

    def process_text(self, text: str) -> TranslationResult:
        context = self._retrieve_context(text)
        translated = self._translate(text, context)
        confidence = Confidence(asr=1.0, translation=0.88, overall=0.94)
        safety = self._check_safety(text, translated, confidence, context)
        cards = self._learning_cards(text, translated, context)
        return TranslationResult(
            session_id=self.session_id,
            language_pair=self.language_pair,
            source_text=text,
            translated_text=translated,
            confidence=confidence,
            safety=safety,
            context_used=context,
            learning_cards=cards,
        )

    def _retrieve_context(self, text: str) -> RetrievedContext:
        lowered = text.lower()
        terms = [term for term in self.glossary if term.source.lower() in lowered]
        safety_terms = [term for term in self.safety_terms if term.lower() in lowered]
        return RetrievedContext(
            profile_id=self.profile_id,
            role="factory_worker",
            glossary_terms=terms,
            safety_terms=safety_terms,
        )

    def _translate(self, text: str, context: RetrievedContext) -> str:
        lowered = text.lower()
        if "dung may" in lowered or "dừng máy" in lowered:
            translated = "Stop the machine and check the safety sensor."
        else:
            translated = "When does this machine need maintenance?"
        if context.glossary_terms:
            hints = ", ".join(f"{term.source} = {term.target}" for term in context.glossary_terms)
            translated = f"{translated} [{hints}]"
        return translated

    def _check_safety(
        self,
        source_text: str,
        translated_text: str,
        confidence: Confidence,
        context: RetrievedContext,
    ) -> SafetyDecision:
        combined = f"{source_text} {translated_text}".lower()
        matched = [term for term in [*self.safety_terms, *context.safety_terms] if term.lower() in combined]
        if matched:
            return SafetyDecision(
                flagged=True,
                severity="critical",
                requires_confirmation=True,
                reason=f"Matched safety term: {matched[0]}",
            )
        if confidence.overall < 0.72:
            return SafetyDecision(
                flagged=True,
                severity="warning",
                requires_confirmation=True,
                reason="Overall confidence below safety threshold",
            )
        return SafetyDecision(False, "none", False, "")

    def _learning_cards(
        self,
        source_text: str,
        translated_text: str,
        context: RetrievedContext,
    ) -> list[LearningCard]:
        if context.glossary_terms:
            return [
                LearningCard(term.source, term.target, "term", source_text)
                for term in context.glossary_terms
            ]
        return [LearningCard(source_text, translated_text, "phrase", source_text)]
