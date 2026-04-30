#include <string>

#include "../src/star_base.h"
#include "my_assert.h"

void test_process_buffer() {
  StarMethod method1{1, 1};
  std::u16string source1(u"12345678");
  std::u16string target1(u"1******8");
  StarBase::ProcessBuffer((skchar_t*)source1.data(), source1.size(), method1);
  MY_ASSERT_ON(source1.compare(target1) == 0);

  StarMethod method2{0, 0};
  std::u16string source2(u"12345678");
  std::u16string target2(u"********");
  StarBase::ProcessBuffer((skchar_t*)source2.data(), source2.size(), method2);
  MY_ASSERT_ON(source2.compare(target2) == 0);
}

void test_process_buffer_at() {
  StarMethod method1{1, 1, u'@'};
  std::u16string source1(u"12345678");
  std::u16string target1(u"1******8");
  StarBase::ProcessBuffer((skchar_t*)source1.data(), source1.size(), method1);
  MY_ASSERT_ON(source1.compare(target1) == 0);

  StarMethod method2{1, 1, u'@'};
  std::u16string source2(u"123456@1");
  std::u16string target2(u"1****6@1");
  StarBase::ProcessBuffer((skchar_t*)source2.data(), source2.size(), method2);
  MY_ASSERT_ON(source2.compare(target2) == 0);

  StarMethod method3{0, 0, u'@'};
  std::u16string source3(u"123456@1");
  std::u16string target3(u"******@1");
  StarBase::ProcessBuffer((skchar_t*)source3.data(), source3.size(), method3);
  MY_ASSERT_ON(source3.compare(target3) == 0);
}

int main(int argc, char* argv[]) {
  test_process_buffer();
  test_process_buffer_at();

  return MY_ASSERT_RESULT();
}
