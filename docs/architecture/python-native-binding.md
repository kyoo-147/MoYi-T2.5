# Python Native Binding

MoYi now exposes a small C ABI bridge in `bindings/native`.

## Current Choice

The first native Python bridge uses `ctypes` over the C ABI library `moyi_c_api`.

Why:

- no mandatory pybind11 dependency for the early alpha
- stable ABI surface for Python and other clients
- easy fallback if Python packaging changes
- close to what Android/JNI also needs: a small native session boundary

## API Surface

```c
moyi_session_handle moyi_create_default_session(void);
const char* moyi_process_text(moyi_session_handle handle, const char* text);
void moyi_free_string(const char* value);
void moyi_destroy_session(moyi_session_handle handle);
```

Python usage:

```powershell
python -m moyi_edge.cli --native --text "Dung may lai, kiem tra cam bien an toan."
```

If the library is not under `build/**`, set:

```powershell
$env:MOYI_C_API_LIBRARY="C:\path\to\moyi_c_api.dll"
```

## Next Step

If the Python SDK grows complex, migrate to pybind11 while keeping the C ABI as a stable low-level bridge.
