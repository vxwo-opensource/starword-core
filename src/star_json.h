#ifndef SJC_STAR_JSON_H_
#define SJC_STAR_JSON_H_

#include "trie_tree.h"

class StarJson final {
  typedef struct StarContext {
    size_t count;
    size_t char_length;
  } StarContext;

 public:
  StarJson(bool skip_number, size_t border);
  ~StarJson();

  bool IsEmpty() const;

  void AddKeyword(const skchar_t* buffer, size_t length);
  bool ProcessBuffer(skchar_t* buffer, size_t length);

 private:
  bool skip_number_;
  size_t border_;
  TrieTree tree_;

  size_t ProcessComplexValue(StarContext& context, skchar_t* buffer,
                             size_t start_index, size_t end_index,
                             bool enter_array);
  size_t ProcessSimpleValue(StarContext& context, skchar_t* buffer,
                            size_t start_index, size_t end_index);

  void StarBuffer(StarContext& context, skchar_t* buffer, size_t start_index,
                  size_t end_index, bool is_number);

  static ssize_t FindKeyEnd(const skchar_t* buffer, size_t start_index,
                            size_t eend_index);
  static ssize_t FindSoftCharEnd(const skchar_t* buffer, skchar_t skip,
                                 skchar_t target, size_t start_index,
                                 size_t end_index);
  static ssize_t FindSymbolEnd(const skchar_t* buffer, size_t start_index,
                               size_t end_index);

  static ssize_t FindNumberEnd(const skchar_t* buffer, size_t start_index,
                               size_t end_index);
};

#endif
