#ifndef SJC_TRIE_TREE_H_
#define SJC_TRIE_TREE_H_

#include "sjc_base.h"

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

  void AddWord(const char16_t* buffer, size_t length, int value);
  void AddWord(char16_t prefix, const char16_t* buffer, size_t length,
               int value);

  bool SearchWord(TrieFound& found, const char16_t* buffer, size_t start_index,
                  size_t end_index) const;

 private:
  TrieNode* root_;

  void InsertWord(TrieNode* node, const char16_t* buffer, size_t length,
                  int value, size_t base_length);

  TrieNode* FindWord(const char16_t* buffer, size_t start_index,
                     size_t end_index) const;
};

#endif
