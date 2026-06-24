cmake_minimum_required(VERSION 3.20)

project(moyi_edge_translation VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MOYI_BUILD_CLI "Build MoYi CLI" ON)
option(MOYI_BUILD_ONNX_RUNTIME "Build ONNX runtime adapter stub" ON)
option(MOYI_BUILD_TESTS "Build MoYi core tests" ON)
option(MOYI_BUILD_C_API "Build MoYi C ABI bridge" ON)
option(MOYI_BUILD_ANDROID_JNI "Build Android JNI bridge when JNI headers are available" OFF)
option(MOYI_ENABLE_REAL_ONNX "Enable ONNX Runtime-backed inference when ONNX Runtime is available" OFF)

include(CTest)

add_library(moyi_core
  core/src/audio/passthrough_audio_frontend.cpp
  core/src/assets/glossary_loader.cpp
  core/src/assets/model_manifest_loader.cpp
  core/src/assets/safety_loader.cpp
  core/src/models/types.cpp
  core/src/context/local_context_retriever.cpp
  core/src/learning/simple_learning_memory.cpp
  core/src/pipeline/translation_session.cpp
  core/src/safety/rule_based_safety_checker.cpp
)
target_include_directories(moyi_core PUBLIC core/include)

add_library(moyi_runtime_mock
  runtimes/mock/src/mock_asr_adapter.cpp
  runtimes/mock/src/mock_translation_adapter.cpp
)
target_include_directories(moyi_runtime_mock PUBLIC runtimes/mock/include)
target_link_libraries(moyi_runtime_mock PUBLIC moyi_core)

if(MOYI_BUILD_ONNX_RUNTIME)
  add_library(moyi_runtime_onnx
    runtimes/onnx/src/onnx_asr_adapter.cpp
    runtimes/onnx/src/onnx_translation_adapter.cpp
  )
  target_include_directories(moyi_runtime_onnx PUBLIC runtimes/onnx/include)
  target_link_libraries(moyi_runtime_onnx PUBLIC moyi_core)
  if(MOYI_ENABLE_REAL_ONNX)
    target_compile_definitions(moyi_runtime_onnx PRIVATE MOYI_ENABLE_REAL_ONNX=1)
    if(DEFINED MOYI_ONNXRUNTIME_ROOT)
      target_include_directories(moyi_runtime_onnx PRIVATE "${MOYI_ONNXRUNTIME_ROOT}/include")
      if(WIN32)
        target_link_libraries(moyi_runtime_onnx PRIVATE "${MOYI_ONNXRUNTIME_ROOT}/lib/onnxruntime.lib")
      else()
        target_link_libraries(moyi_runtime_onnx PRIVATE "${MOYI_ONNXRUNTIME_ROOT}/lib/libonnxruntime.so")
      endif()
    endif()
  endif()
endif()

if(MOYI_BUILD_CLI)
  add_executable(moyi apps/cli/src/main.cpp)
  target_link_libraries(moyi PRIVATE moyi_core moyi_runtime_mock)
endif()

if(MOYI_BUILD_C_API)
  add_library(moyi_c_api SHARED bindings/native/moyi_c_api.cpp)
  target_include_directories(moyi_c_api PUBLIC bindings/native)
  target_link_libraries(moyi_c_api PRIVATE moyi_core moyi_runtime_mock)
endif()

if(MOYI_BUILD_ANDROID_JNI)
  find_package(JNI REQUIRED)
  add_library(moyi_android_jni SHARED bindings/android-jni/src/moyi_jni.cpp)
  target_include_directories(moyi_android_jni PRIVATE bindings/android-jni/include ${JNI_INCLUDE_DIRS})
  target_link_libraries(moyi_android_jni PRIVATE moyi_core moyi_runtime_mock ${JNI_LIBRARIES})
endif()

if(MOYI_BUILD_TESTS AND BUILD_TESTING)
  function(moyi_add_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE moyi_core moyi_runtime_mock)
    add_test(NAME ${name} COMMAND ${name})
  endfunction()

  moyi_add_test(test_types core/tests/test_types.cpp)
  moyi_add_test(test_context_retriever core/tests/test_context_retriever.cpp)
  moyi_add_test(test_safety_checker core/tests/test_safety_checker.cpp)
  moyi_add_test(test_learning_memory core/tests/test_learning_memory.cpp)
  moyi_add_test(test_translation_session core/tests/test_translation_session.cpp)
endif()
