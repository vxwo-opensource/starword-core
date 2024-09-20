#ifndef SJC_TRIE_TREE_H_
#define SJC_TRIE_TREE_H_

#include "skc_base.h"

typedef struct TrieFound {
  int value;
  size_t start_index;
  size_t stop_index;
} TrieIndex;

class TrieTree final {
  struct TrieNode;

 public:
  TrieTree();
  ~TrieTree();

  bool IsEmpty() const;

  void AddWord(const skchar_t* buffer, size_t length, int value);
  void AddWord(skchar_t prefix, const skchar_t* buffer, size_t length,
               int value);

  bool SearchWord(TrieFound& found, const skchar_t* buffer, size_t start_index,
                  size_t end_index) const;

 private:
  TrieNode* root_;

  void InsertWord(TrieNode* node, const skchar_t* buffer, size_t length,
                  int value, size_t base_length);

  TrieNode* FindWord(const skchar_t* buffer, size_t start_index,
                     size_t end_index) const;
};

#endif
