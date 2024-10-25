#include <string>

#include "../src/star_json.h"
#include "my_assert.h"

const std::u16string word_phone(u"phone");

void setupNormalEngine(StarJson& engine) {
  engine.AddWord((skchar_t*)word_phone.data(), word_phone.size());
  engine.FinishAdd();
}

void test_process_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"12345678\"}");
  std::u16string target(u"{\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false20() {
  StarOptions options{false, 2, 0};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"12345678\"}");
  std::u16string target(u"{\"phone\":\"12******\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false02() {
  StarOptions options{false, 0, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"12345678\"}");
  std::u16string target(u"{\"phone\":\"******78\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true11() {
  StarOptions options{true, 1, 1};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"PhoNE\":\"12345678\"}");
  std::u16string target(u"{\"PhoNE\":\"1******8\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true00() {
  StarOptions options{true, 0, 0};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"PhoNE\":\"12345678\"}");
  std::u16string target(u"{\"PhoNE\":\"********\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true21() {
  StarOptions options{true, 2, 1};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"PhoNE\":\"12345678\"}");
  std::u16string target(u"{\"PhoNE\":\"12*****8\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());

  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_ignore_true00() {
  StarOptions options{true, 2, 1};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"mobile\":\"12345678\"}");
  std::u16string target(u"{\"mobile\":\"12345678\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_empty_false22() {
  StarOptions options{true, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"\"}");
  std::u16string target(u"{\"phone\":\"\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_array_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":[\"12345678\",\"12345678\"]}");
  std::u16string target(u"{\"phone\":[\"12****78\",\"12****78\"]}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_level_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":{\"phone\":\"12345678\"}}");
  std::u16string target(u"{\"phone\":{\"phone\":\"12****78\"}}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_multiple_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source1(u"{\"phone\":\"12345678\",\"phone\":\"12345678\"}");
  std::u16string target1(u"{\"phone\":\"12****78\",\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source1.data(), source1.size());
  MY_ASSERT_ON(source1.compare(target1) == 0);

  std::u16string source2(u"{\"phone\":null,\"phone\":\"12345678\"}");
  std::u16string target2(u"{\"phone\":null,\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source2.data(), source2.size());
  MY_ASSERT_ON(source2.compare(target2) == 0);

  std::u16string source3(u"{\"phone\",\"phone\":\"12345678\"}");
  std::u16string target3(u"{\"phone\",\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source3.data(), source3.size());
  MY_ASSERT_ON(source3.compare(target3) == 0);

  std::u16string source4(u"{\"phone\":\"phone\",\"phone\":\"12345678\"}");
  std::u16string target4(u"{\"phone\":\"ph*ne\",\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source4.data(), source4.size());
  MY_ASSERT_ON(source4.compare(target4) == 0);

  std::u16string source5(u"{\"phone\":,\"phone\":\"12345678\"}");
  std::u16string target5(u"{\"phone\":,\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source5.data(), source5.size());
  MY_ASSERT_ON(source5.compare(target5) == 0);
}

void test_process_inline_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\": {\\\"phone\\\": \\\"12345678\\\"}}");
  std::u16string target(u"{\"phone\": {\\\"phone\\\": \\\"12****78\\\"}}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_double_quote_first_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"\\\"12345678\"}");
  std::u16string target(u"{\"phone\":\"\\\"******78\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_double_quote_middle_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"12345\\\"678\"}");
  std::u16string target(u"{\"phone\":\"12******78\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_double_quote_end_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"12345678\\\"\"}");
  std::u16string target(u"{\"phone\":\"12******\\\"\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_back_slash_on_end_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"12345678\\\\\"}");
  std::u16string target(u"{\"phone\":\"12******\\\\\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_back_slash_only_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"{\"phone\":\"\\\\\\\\\\\\\"}");
  std::u16string target(u"{\"phone\":\"\\\\**\\\\\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

int main(int argc, char* argv[]) {
  test_process_false22();
  test_process_false20();
  test_process_false02();
  test_process_true11();
  test_process_true00();
  test_process_true21();
  test_ignore_true00();
  test_process_empty_false22();
  test_process_array_false22();
  test_process_level_false22();
  test_process_multiple_false22();
  test_process_inline_false22();
  test_process_escape_double_quote_first_false22();
  test_process_escape_double_quote_middle_false22();
  test_process_escape_double_quote_end_false22();
  test_process_escape_back_slash_on_end_false22();
  test_process_escape_back_slash_only_false22();

  return MY_ASSERT_RESULT();
}
