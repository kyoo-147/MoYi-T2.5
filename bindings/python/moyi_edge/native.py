from __future__ import annotations

import ctypes
import json
import os
from pathlib import Path
from typing import Any


def _candidate_library_names() -> list[str]:
    if os.name == "nt":
        return ["moyi_c_api.dll"]
    if os.uname().sysname == "Darwin":
        return ["libmoyi_c_api.dylib"]
    return ["libmoyi_c_api.so"]


def _find_library() -> Path:
    explicit = os.environ.get("MOYI_C_API_LIBRARY")
    if explicit:
        return Path(explicit)
    root = Path(__file__).resolve().parents[3]
    candidates: list[Path] = []
    for name in _candidate_library_names():
        candidates.extend(root.glob(f"build/**/{name}"))
    if not candidates:
        raise FileNotFoundError(
            "MoYi native library not found. Build the CMake target moyi_c_api or set MOYI_C_API_LIBRARY."
        )
    return candidates[0]


class NativeMoYiPipeline:
    def __init__(self) -> None:
        library_path = _find_library()
        self._lib = ctypes.CDLL(str(library_path))
        self._lib.moyi_create_default_session.restype = ctypes.c_void_p
        self._lib.moyi_process_text.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        self._lib.moyi_process_text.restype = ctypes.c_void_p
        self._lib.moyi_free_string.argtypes = [ctypes.c_void_p]
        self._lib.moyi_destroy_session.argtypes = [ctypes.c_void_p]
        self._handle = self._lib.moyi_create_default_session()

    def process_text(self, text: str) -> dict[str, Any]:
        raw_ptr = self._lib.moyi_process_text(self._handle, text.encode("utf-8"))
        try:
            payload = ctypes.cast(raw_ptr, ctypes.c_char_p).value
            if payload is None:
                raise RuntimeError("Native MoYi returned a null payload")
            return json.loads(payload.decode("utf-8"))
        finally:
            self._lib.moyi_free_string(raw_ptr)

    def close(self) -> None:
        if getattr(self, "_handle", None):
            self._lib.moyi_destroy_session(self._handle)
            self._handle = None

    def __enter__(self) -> "NativeMoYiPipeline":
        return self

    def __exit__(self, *_: object) -> None:
        self.close()
