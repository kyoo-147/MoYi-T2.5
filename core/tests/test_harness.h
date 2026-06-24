#pragma once

#include <cstdlib>
#include <iostream>
#include <string>

#define MOYI_ASSERT_TRUE(condition)                                                      \
  do {                                                                                    \
    if (!(condition)) {                                                                   \
      std::cerr << "Assertion failed: " #condition << " at " << __FILE__ << ":"          \
                << __LINE__ << "\n";                                                     \
      return EXIT_FAILURE;                                                                \
    }                                                                                     \
  } while (false)

#define MOYI_ASSERT_EQ(actual, expected)                                                  \
  do {                                                                                    \
    const auto actual_value = (actual);                                                   \
    const auto expected_value = (expected);                                               \
    if (!(actual_value == expected_value)) {                                              \
      std::cerr << "Assertion failed: " #actual " == " #expected << " at " << __FILE__   \
                << ":" << __LINE__ << "\nactual: " << actual_value                       \
                << "\nexpected: " << expected_value << "\n";                             \
      return EXIT_FAILURE;                                                                \
    }                                                                                     \
  } while (false)
