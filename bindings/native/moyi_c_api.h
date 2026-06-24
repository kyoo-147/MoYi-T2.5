#pragma once

#ifdef _WIN32
#define MOYI_C_API __declspec(dllexport)
#else
#define MOYI_C_API
#endif

extern "C" {

typedef void* moyi_session_handle;

MOYI_C_API moyi_session_handle moyi_create_default_session(void);
MOYI_C_API const char* moyi_process_text(moyi_session_handle handle, const char* text);
MOYI_C_API void moyi_free_string(const char* value);
MOYI_C_API void moyi_destroy_session(moyi_session_handle handle);

}
