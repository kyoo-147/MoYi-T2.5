#include "moyi_jni.h"

#include <memory>
#include <string>

namespace {

struct NativeSessionPlaceholder {
  std::string profile_id{"factory_worker"};
};

}  // namespace

JNIEXPORT jlong JNICALL
Java_ai_moyi_edge_MoYiNative_createSession(JNIEnv*, jobject) {
  auto* session = new NativeSessionPlaceholder();
  return reinterpret_cast<jlong>(session);
}

JNIEXPORT jstring JNICALL
Java_ai_moyi_edge_MoYiNative_processText(JNIEnv* env, jobject, jlong handle, jstring text) {
  if (handle == 0 || text == nullptr) {
    return env->NewStringUTF("{\"error\":\"invalid_session_or_text\"}");
  }

  const char* raw = env->GetStringUTFChars(text, nullptr);
  std::string source = raw == nullptr ? "" : raw;
  env->ReleaseStringUTFChars(text, raw);

  std::string payload =
      "{\"session\":\"android-placeholder\",\"source_text\":\"" + source +
      "\",\"message\":\"JNI boundary is ready; native session wiring comes next\"}";
  return env->NewStringUTF(payload.c_str());
}

JNIEXPORT void JNICALL
Java_ai_moyi_edge_MoYiNative_destroySession(JNIEnv*, jobject, jlong handle) {
  auto* session = reinterpret_cast<NativeSessionPlaceholder*>(handle);
  delete session;
}
