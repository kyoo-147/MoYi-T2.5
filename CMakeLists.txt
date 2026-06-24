cmake_minimum_required(VERSION 3.20)

project(moyi_edge_translation VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MOYI_BUILD_CLI "Build MoYi CLI" ON)
option(MOYI_BUILD_ONNX_RUNTIME "Build ONNX runtime adapter stub" ON)

add_library(moyi_core
  core/src/audio/passthrough_audio_frontend.cpp
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
endif()

if(MOYI_BUILD_CLI)
  add_executable(moyi apps/cli/src/main.cpp)
  target_link_libraries(moyi PRIVATE moyi_core moyi_runtime_mock)
endif()
