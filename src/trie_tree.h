#ifndef SKC_TRIE_TREE_H_
#define SKC_TRIE_TREE_H_

#include "skc_base.h"

struct TrieFound {
  size_t start_index;
  size_t stop_index;
};

struct TrieNode;
class TrieTree final {
 public:
  TrieTree(bool ignore_case);
  ~TrieTree();

  bool IsEmpty() const;

  void AddWord(const skchar_t* buffer, size_t length);
  void AddWord(skchar_t prefix, const skchar_t* buffer, size_t length);
  void FinishAdd();

  bool SearchWord(TrieFound& found, const skchar_t* buffer, size_t start_index,
                  size_t stop_index) const;

 private:
  TrieNode* root_;
  bool ignore_case_;

  void InsertWord(TrieNode* node, const skchar_t* buffer, size_t length,
                  size_t extra_length);
};

#endif
