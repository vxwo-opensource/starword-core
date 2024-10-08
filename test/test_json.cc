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
}

int main(int argc, char* argv[]) {
  test_process_false22();
  test_process_false20();
  test_process_false02();
  test_process_true11();
  test_process_true00();
  test_process_true21();
  test_ignore_true00();

  test_process_number_false22();

  return MY_ASSERT_RESULT();
}
