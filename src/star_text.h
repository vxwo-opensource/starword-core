#ifndef SKC_STAR_TEXT_H_
#define SKC_STAR_TEXT_H_

#include "star_base.h"

struct StarTextOptions {
  bool ignore_case;
  size_t left_border;
  size_t right_border;
};

class StarText : public StarBase {
 public:
  StarText(const StarTextOptions& options);

  bool ProcessBuffer(skchar_t* buffer, size_t length) override;

 private:
  StarMethod method_;
};

#endif
