#ifndef SKC_STAR_TEXT_H_
#define SKC_STAR_TEXT_H_

#include "trie_tree.h"

class StarText final {
  typedef struct StarContext {
    size_t count;
    size_t char_length;
  } StarContext;

 public:
  StarText(size_t border);
  ~StarText();

  bool IsEmpty() const;

  void AddKeyword(const skchar_t* buffer, size_t length);
  bool ProcessBuffer(skchar_t* buffer, size_t length);

 private:
  size_t border_;
  TrieTree tree_;

  void StarBuffer(StarContext& context, skchar_t* buffer, size_t start_index,
                  size_t end_index);
};

#endif
