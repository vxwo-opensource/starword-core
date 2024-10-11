#include <string>

#include "../src/star_text.h"
#include "my_assert.h"

const std::u16string keyword_phone(u"phone");
const std::u16string keyword_utf16_4bytes(u"𝄞𝄞𝄞𝄞𝄞");

void test_process_false22() {
  StarOptions options{false, 2, 2};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::u16string source(u"i am a phone");
  std::u16string target(u"i am a ph*ne");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false20() {
  StarOptions options{false, 2, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::u16string source(u"i am a phone");
  std::u16string target(u"i am a ph***");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false02() {
  StarOptions options{false, 0, 2};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::u16string source(u"i am a phone");
  std::u16string target(u"i am a ***ne");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true11() {
  StarOptions options{true, 1, 1};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::u16string source(u"i am a PhoNE");
  std::u16string target(u"i am a P***E");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true00() {
  StarOptions options{true, 0, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::u16string source(u"i am a PhoNE");
  std::u16string target(u"i am a *****");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true21() {
  StarOptions options{true, 2, 1};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::u16string source(u"i am a PhoNE");
  std::u16string target(u"i am a Ph**E");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_ignore_true00() {
  StarOptions options{true, 0, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());

  std::u16string source(u"i am a mobile");
  std::u16string target(u"i am a mobile");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_utf16_4bytes_middle_fasle00() {
  StarOptions options{false, 0, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_utf16_4bytes.data(),
                    keyword_utf16_4bytes.size());

  std::u16string source(u"before 𝄞𝄞𝄞𝄞𝄞 after");
  std::u16string target(u"before ********** after");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_utf16_4bytes_before_fasle10() {
  StarOptions options{false, 1, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_utf16_4bytes.data(),
                    keyword_utf16_4bytes.size());

  for(int i = 0;i<keyword_utf16_4bytes.size(); ++i) {
    printf("keyword: %0X\n", keyword_utf16_4bytes.data()[i]);

  }
  std::u16string source(u"𝄞𝄞𝄞𝄞𝄞 after");
  std::u16string target(u"𝄞******** after");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_utf16_4bytes_after_fasle01() {
  StarOptions options{false, 0, 1};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_utf16_4bytes.data(),
                    keyword_utf16_4bytes.size());

  for(int i = 0;i<keyword_utf16_4bytes.size(); ++i) {
    printf("keyword: %0X\n", keyword_utf16_4bytes.data()[i]);

  }
  std::u16string source(u"before 𝄞𝄞𝄞𝄞𝄞");
  std::u16string target(u"before ********𝄞");
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

  test_process_utf16_4bytes_middle_fasle00();
  test_process_utf16_4bytes_before_fasle10();
  test_process_utf16_4bytes_after_fasle01();

  return MY_ASSERT_RESULT();
}
