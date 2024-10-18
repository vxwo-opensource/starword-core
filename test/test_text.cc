#include <string>

#include "../src/star_text.h"
#include "my_assert.h"

const std::u16string keyword_phone(u"phone");
const std::u16string keyword_utf16_4bytes(u"𝄞𝄞𝄞𝄞𝄞");

void setupNormalEngine(StarText& engine) {
  engine.AddKeyword((skchar_t*)keyword_phone.data(), keyword_phone.size());
  engine.AddKeyword((skchar_t*)keyword_utf16_4bytes.data(),
                    keyword_utf16_4bytes.size());
  engine.FinishAdd();
}

void test_process_false22() {
  StarOptions options{false, 2, 2};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"i am a phone");
  std::u16string target(u"i am a ph*ne");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false20() {
  StarOptions options{false, 2, 0};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"i am a phone");
  std::u16string target(u"i am a ph***");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_false02() {
  StarOptions options{false, 0, 2};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"i am a phone");
  std::u16string target(u"i am a ***ne");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true11() {
  StarOptions options{true, 1, 1};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"i am a PhoNE");
  std::u16string target(u"i am a P***E");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true00() {
  StarOptions options{true, 0, 0};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"i am a PhoNE");
  std::u16string target(u"i am a *****");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_true21() {
  StarOptions options{true, 2, 1};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"i am a PhoNE");
  std::u16string target(u"i am a Ph**E");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_ignore_true00() {
  StarOptions options{true, 0, 0};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"i am a mobile");
  std::u16string target(u"i am a mobile");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_utf16_4bytes_middle_fasle00() {
  StarOptions options{false, 0, 0};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"before 𝄞𝄞𝄞𝄞𝄞 after");
  std::u16string target(u"before ********** after");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_utf16_4bytes_before_fasle10() {
  StarOptions options{false, 1, 0};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"𝄞𝄞𝄞𝄞𝄞 after");
  std::u16string target(u"𝄞******** after");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_process_utf16_4bytes_after_fasle01() {
  StarOptions options{false, 0, 1};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source(u"before 𝄞𝄞𝄞𝄞𝄞");
  std::u16string target(u"before ********𝄞");
  engine.ProcessBuffer((skchar_t*)source.data(), source.size());
  MY_ASSERT_ON(source.compare(target) == 0);
}

void test_skip_length_sample() {
  StarOptions options{false, 0, 0};
  StarText engine(options);
  setupNormalEngine(engine);

  std::u16string source1(u"phonphonphone");
  std::u16string target1(u"phonphon*****");
  engine.ProcessBuffer((skchar_t*)source1.data(), source1.size());
  MY_ASSERT_ON(source1.compare(target1) == 0);

  std::u16string source2(u"ppphone");
  std::u16string target2(u"pp*****");
  engine.ProcessBuffer((skchar_t*)source2.data(), source2.size());
  MY_ASSERT_ON(source2.compare(target2) == 0);

  std::u16string source3(u"phphphone");
  std::u16string target3(u"phph*****");
  engine.ProcessBuffer((skchar_t*)source3.data(), source3.size());
  MY_ASSERT_ON(source3.compare(target3) == 0);
}

void test_skip_length_complex() {
  const std::u16string keyword_short1(u"def");
  const std::u16string keyword_short2(u"abcd");
  const std::u16string keyword_long(u"abcdefg");

  StarOptions options{false, 0, 0};
  StarText engine(options);
  engine.AddKeyword((skchar_t*)keyword_short1.data(), keyword_short1.size());
  engine.AddKeyword((skchar_t*)keyword_short2.data(), keyword_short2.size());
  engine.AddKeyword((skchar_t*)keyword_long.data(), keyword_long.size());
  engine.FinishAdd();

  std::u16string source1(u"aabcdefgg");
  std::u16string target1(u"a*******g");
  engine.ProcessBuffer((skchar_t*)source1.data(), source1.size());
  MY_ASSERT_ON(source1.compare(target1) == 0);

  std::u16string source2(u"abcdeffdefabcggabcgg");
  std::u16string target2(u"****eff***abcggabcgg");
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

  test_process_utf16_4bytes_middle_fasle00();
  test_process_utf16_4bytes_before_fasle10();
  test_process_utf16_4bytes_after_fasle01();

  test_skip_length_sample();
  test_skip_length_complex();

  return MY_ASSERT_RESULT();
}
