#include <string>

#include "../src/star_json.h"
#include "my_assert.h"

using std::wstring;

const wstring keyword_phone(L"phone");

void test_process_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":\"12345678\"}");
  std::wstring target(L"{\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false20() {
  StarOptions options{false, 2, 0};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":\"12345678\"}");
  std::wstring target(L"{\"phone\":\"12******\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false02() {
  StarOptions options{false, 0, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":\"12345678\"}");
  std::wstring target(L"{\"phone\":\"******78\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true11() {
  StarOptions options{true, 1, 1};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"PhoNE\":\"12345678\"}");
  std::wstring target(L"{\"PhoNE\":\"1******8\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true00() {
  StarOptions options{true, 0, 0};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"PhoNE\":\"12345678\"}");
  std::wstring target(L"{\"PhoNE\":\"********\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true21() {
  StarOptions options{true, 2, 1};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"PhoNE\":\"12345678\"}");
  std::wstring target(L"{\"PhoNE\":\"12*****8\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());

  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_ignore_true00() {
  StarOptions options{true, 2, 1};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"mobile\":\"12345678\"}");
  std::wstring target(L"{\"mobile\":\"12345678\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_array_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":[\"12345678\",\"12345678\"]}");
  std::wstring target(L"{\"phone\":[\"12****78\",\"12****78\"]}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_number_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, false);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source1(L"{\"phone\": 12345678}");
  std::wstring target1(L"{\"phone\": 12****78}");
  engine.ProcessBuffer((skchar_t*)source1.data(), source1.size());
  MY_ASSERT_ON(source1.compare(target1) == 0);

  std::wstring source2(L"{\"phone\": -}");
  std::wstring target2(L"{\"phone\": *}");
  engine.ProcessBuffer((skchar_t*)source2.data(), source2.size());
  MY_ASSERT_ON(source2.compare(target2) == 0);

  std::wstring source3(L"{\"phone\": -....}");
  std::wstring target3(L"{\"phone\": -.*..}");
  engine.ProcessBuffer((skchar_t*)source3.data(), source3.size());
  MY_ASSERT_ON(source3.compare(target3) == 0);
}

void test_process_number_array_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, false);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":[12345678,,[12345678]}");
  std::wstring target(L"{\"phone\":[12****78,,[12345678]}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_level_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":{\"phone\":\"12345678\"}}");
  std::wstring target(L"{\"phone\":{\"phone\":\"12****78\"}}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_multiple_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source1(L"{\"phone\":\"12345678\",\"phone\":\"12345678\"}");
  std::wstring target1(L"{\"phone\":\"12****78\",\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source1.data(), source1.size());
  MY_ASSERT_ON(source1.compare(target1) == 0);

  std::wstring source2(L"{\"phone\":null,\"phone\":\"12345678\"}");
  std::wstring target2(L"{\"phone\":null,\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source2.data(), source2.size());
  MY_ASSERT_ON(source2.compare(target2) == 0);

  std::wstring source3(L"{\"phone\",\"phone\":\"12345678\"}");
  std::wstring target3(L"{\"phone\",\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source3.data(), source3.size());
  MY_ASSERT_ON(source3.compare(target3) == 0);

  std::wstring source4(L"{\"phone\":\"phone\",\"phone\":\"12345678\"}");
  std::wstring target4(L"{\"phone\":\"ph*ne\",\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source4.data(), source4.size());
  MY_ASSERT_ON(source4.compare(target4) == 0);

  std::wstring source5(L"{\"phone\":,\"phone\":\"12345678\"}");
  std::wstring target5(L"{\"phone\":,\"phone\":\"12****78\"}");
  engine.ProcessBuffer((skchar_t*)source5.data(), source5.size());
  MY_ASSERT_ON(source5.compare(target5) == 0);
}

void test_process_inline_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\": {\\\"phone\\\": \\\"12345678\\\"}}");
  std::wstring target(L"{\"phone\": {\\\"phone\\\": \\\"12****78\\\"}}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_double_quote_first_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":\"\\\"12345678\"}");
  std::wstring target(L"{\"phone\":\"\\\"******78\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_double_quote_middle_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":\"12345\\\"678\"}");
  std::wstring target(L"{\"phone\":\"12******78\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_double_quote_end_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":\"12345678\\\"\"}");
  std::wstring target(L"{\"phone\":\"12******\\\"\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_back_slash_on_end_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":\"12345678\\\\\"}");
  std::wstring target(L"{\"phone\":\"12******\\\\\"}");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_escape_back_slash_only_false22() {
  StarOptions options{false, 2, 2};
  StarJson engine(options, true);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"{\"phone\":\"\\\\\\\\\\\\\"}");
  std::wstring target(L"{\"phone\":\"\\\\**\\\\\"}");
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
  test_process_array_false22();
  test_process_number_false22();
  test_process_number_array_false22();
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
