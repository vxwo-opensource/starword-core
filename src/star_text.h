#ifndef SKC_STAR_TEXT_H_
#define SKC_STAR_TEXT_H_

#include "star_base.h"

class StarText : public StarBase {
 public:
  StarText(bool ignore_case, size_t left_border, size_t right_border);

  bool ProcessBuffer(skchar_t* buffer, size_t length);

 private:
  size_t left_border_;
  size_t right_border_;

  void StarBuffer(StarContext& context, skchar_t* buffer, size_t start_index,
                  size_t end_index);
};

#endif
