#ifndef SKC_STAR_TEXT_H_
#define SKC_STAR_TEXT_H_

#include "star_base.h"

class StarText : public StarBase {
 public:
  StarText(const StarOptions& options);

  bool ProcessBuffer(skchar_t* buffer, size_t length);
};

#endif
