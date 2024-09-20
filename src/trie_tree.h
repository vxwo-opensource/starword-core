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

  void AddWord(const sjchar_t* buffer, size_t length, int value);
  void AddWord(sjchar_t prefix, const sjchar_t* buffer, size_t length,
               int value);

  bool SearchWord(TrieFound& found, const sjchar_t* buffer, size_t start_index,
                  size_t end_index) const;

 private:
  TrieNode* root_;

  void InsertWord(TrieNode* node, const sjchar_t* buffer, size_t length,
                  int value, size_t base_length);

  TrieNode* FindWord(const sjchar_t* buffer, size_t start_index,
                     size_t end_index) const;
};

#endif
