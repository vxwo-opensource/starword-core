#ifndef STAR_JSON_H_
#define STAR_JSON_H_

#include "jwbase.h"
#include "trie_tree.h"

class StarJson final {
  typedef struct StarContext {
    size_t count;
    size_t char_length;
  } StarContext;

 public:
  StarJson(bool skip_number, size_t border);
  ~StarJson();

  bool IsEmpty() const { return tree_.IsEmpty(); };

  void AddPrefix(const JWCharBuffer buffer, size_t length);
  bool ProcessBuffer(JWCharBuffer buffer, size_t length);

 private:
  bool skip_number_;
  size_t border_;
  TrieTree tree_;

  size_t ProcessComplexValue(StarContext &context, JWCharBuffer buffer,
                             size_t start_index, size_t end_index,
                             bool enter_array);
  size_t ProcessSimpleValue(StarContext &context, JWCharBuffer buffer,
                            size_t start_index, size_t end_index);

  void StarBuffer(StarContext &context, JWCharBuffer buffer, size_t start_index,
                  size_t end_index, bool is_number);

  static ssize_t FindKeyEnd(const JWCharBuffer buffer, size_t start_index,
                            size_t eend_index);
  static ssize_t FindSoftCharEnd(const JWCharBuffer buffer, JWChar skip,
                                 JWChar target, size_t start_index,
                                 size_t end_index);
  static ssize_t FindSymbolEnd(const JWCharBuffer buffer, size_t start_index,
                               size_t end_index);

  static ssize_t FindNumberEnd(const JWCharBuffer buffer, size_t start_index,
                               size_t end_index);
};

#endif
