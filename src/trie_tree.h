#ifndef TRIE_TREE_H_
#define TRIE_TREE_H_

#include "jwbase.h"

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

  void AddWord(const JWCharBuffer buffer, size_t length, int value);
  void AddWord(JWChar prefix, const JWCharBuffer buffer, size_t length,
               int value);

  bool SearchWord(TrieFound &found, const JWCharBuffer buffer,
                  size_t start_index, size_t end_index) const;

 private:
  TrieNode *root_;

  void InsertWord(TrieNode *node, const JWCharBuffer buffer, size_t length,
                  int value, size_t base_length);

  TrieNode *FindWord(const JWCharBuffer buffer, size_t start_index,
                     size_t end_index) const;
};

#endif
