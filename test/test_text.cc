#include <string>

#include "../src/star_text.h"
#include "my_assert.h"

using std::wstring;

const wstring keyword_phone(L"phone");

void test_process_false22() {
  StarOptions options{false, 2, 2};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"i am a phone");
  std::wstring target(L"i am a ph*ne");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());

  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false20() {
  StarOptions options{false, 2, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"i am a phone");
  std::wstring target(L"i am a ph***");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());

  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false02() {
  StarOptions options{false, 0, 2};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"i am a phone");
  std::wstring target(L"i am a ***ne");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());

  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true11() {
  StarOptions options{true, 1, 1};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"i am a PhoNE");
  std::wstring target(L"i am a P***E");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());

  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true00() {
  StarOptions options{true, 0, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"i am a PhoNE");
  std::wstring target(L"i am a *****");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());

  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true21() {
  StarOptions options{true, 2, 1};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"i am a PhoNE");
  std::wstring target(L"i am a Ph**E");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());

  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_ignore_true00() {
  StarOptions options{true, 0, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::wstring source(L"i am a mobile");
  std::wstring target(L"i am a mobile");
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

  return MY_ASSERT_RESULT();
}
