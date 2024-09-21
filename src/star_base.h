#ifndef SKC_STAR_BASE_H_
#define SKC_STAR_BASE_H_

#include "trie_tree.h"

struct StarContext {
  size_t count;
  size_t char_length;
};

class StarBase {
 public:
  virtual ~StarBase() = 0;

  bool IsEmpty() const;

  virtual void AddKeyword(const skchar_t* buffer, size_t length);
  virtual bool ProcessBuffer(skchar_t* buffer, size_t length) = 0;

 protected:
  TrieTree tree_;

  StarBase(bool ignore_case);
};

#endif
