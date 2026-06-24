#include "test_harness.h"

#include "moyi/types.h"

int main() {
  MOYI_ASSERT_EQ(moyi::to_string(moyi::Language::Vietnamese), std::string("vi"));
  MOYI_ASSERT_EQ(moyi::to_string(moyi::Language::English), std::string("en"));
  MOYI_ASSERT_TRUE(moyi::language_from_code("vi") == moyi::Language::Vietnamese);
  MOYI_ASSERT_TRUE(moyi::language_from_code("ko") == moyi::Language::Korean);

  const moyi::LanguagePair pair{moyi::Language::Vietnamese, moyi::Language::English};
  MOYI_ASSERT_EQ(pair.code(), std::string("vi-en"));
  return EXIT_SUCCESS;
}
