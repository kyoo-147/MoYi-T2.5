#pragma once

#include <jni.h>

extern "C" {

JNIEXPORT jlong JNICALL
Java_ai_moyi_edge_MoYiNative_createSession(JNIEnv* env, jobject thiz);

JNIEXPORT jstring JNICALL
Java_ai_moyi_edge_MoYiNative_processText(JNIEnv* env, jobject thiz, jlong handle, jstring text);

JNIEXPORT void JNICALL
Java_ai_moyi_edge_MoYiNative_destroySession(JNIEnv* env, jobject thiz, jlong handle);

}
