#pragma once

#include <string>
#include <utility>
#include <vector>

#include "moyi/types.h"

namespace moyi {

struct TranslationResult {
  std::string session_id;
  LanguagePair language_pair;
  std::string source_text;
  std::string translated_text;
  Confidence confidence;
  SafetyDecision safety;
  RetrievedContext context_used;
  std::vector<LearningCard> learning_cards;
};

struct RuntimeError {
  std::string code;
  std::string message;
};

template <typename T>
class Result {
 public:
  static Result<T> ok(T value) { return Result<T>(std::move(value)); }
  static Result<T> error(RuntimeError error) { return Result<T>(std::move(error)); }

  [[nodiscard]] bool is_ok() const { return ok_; }
  [[nodiscard]] const T& value() const { return value_; }
  [[nodiscard]] T& value() { return value_; }
  [[nodiscard]] const RuntimeError& error() const { return error_; }

 private:
  explicit Result(T value) : ok_(true), value_(std::move(value)) {}
  explicit Result(RuntimeError error) : ok_(false), error_(std::move(error)) {}

  bool ok_{false};
  T value_{};
  RuntimeError error_{};
};

template <>
class Result<void> {
 public:
  static Result<void> ok() { return Result<void>(); }
  static Result<void> error(RuntimeError error) { return Result<void>(std::move(error)); }

  [[nodiscard]] bool is_ok() const { return ok_; }
  [[nodiscard]] const RuntimeError& error() const { return error_; }

 private:
  Result() : ok_(true) {}
  explicit Result(RuntimeError error) : ok_(false), error_(std::move(error)) {}

  bool ok_{false};
  RuntimeError error_{};
};

}  // namespace moyi
