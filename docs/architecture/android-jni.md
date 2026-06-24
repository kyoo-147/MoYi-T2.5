# Android JNI Bridge

The Android JNI layer now owns a real native `TranslationSession` instead of returning placeholder text.

## Native Methods

```cpp
Java_ai_moyi_edge_MoYiNative_createSession
Java_ai_moyi_edge_MoYiNative_processText
Java_ai_moyi_edge_MoYiNative_destroySession
```

## Behavior

- `createSession` constructs a C++ MoYi session with mock runtime adapters and factory safety/glossary defaults.
- `processText` runs `TranslationSession::process_once`.
- The returned payload follows the same JSON contract used by the CLI and C ABI.
- `destroySession` releases the native session.

## Build

JNI build is opt-in because desktop environments may not have JNI headers:

```powershell
cmake -S . -B build/android-jni -DMOYI_BUILD_ANDROID_JNI=ON
```

The next Android phase should wire this library into a minimal Kotlin wrapper and Gradle/CMake Android build.
