#ifndef SJC_STAR_JSON_H_
#define SJC_STAR_JSON_H_

#include "star_base.h"

class StarJson : public StarBase {
 public:
  StarJson(const StarOptions& options);

  void AddWord(const skchar_t* buffer, size_t length) override;
  bool ProcessBuffer(skchar_t* buffer, size_t length) override;

 protected:
  void StarBuffer(StarContext& context, skchar_t* buffer, size_t start_index,
                  size_t stop_index) override;

 private:
  size_t ProcessComplexValue(StarContext& context, skchar_t* buffer,
                             size_t start_index, size_t stop_index,
                             bool enter_array);
  size_t ProcessSimpleValue(StarContext& context, skchar_t* buffer,
                            size_t start_index, size_t stop_index);

  static ssize_t FindKeyEnd(const skchar_t* buffer, size_t start_index,
                            size_t stop_index);
  static ssize_t FindSoftCharEnd(const skchar_t* buffer, skchar_t skip,
                                 skchar_t target, size_t start_index,
                                 size_t stop_index);
  static ssize_t FindSymbolEnd(const skchar_t* buffer, size_t start_index,
                               size_t stop_index);

  static ssize_t FindNumberEnd(const skchar_t* buffer, size_t start_index,
                               size_t stop_index);
};

#endif
