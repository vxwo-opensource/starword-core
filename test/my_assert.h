
#ifndef MY_ASSERT_H
#define MY_ASSERT_H

namespace my {
  int assert_failed();
  
  void assert_output(bool passed, const char* expr, const char* func, const char* file, int line);
}

#define MY_ASSERT_ON(expr) \
  my::assert_output(!!(expr), #expr, __func__, __FILE__, __LINE__)

#define MY_ASSERT_RESULT() \
   (my::assert_failed()>0?1:0)

#endif // MY_ASSERT_H
