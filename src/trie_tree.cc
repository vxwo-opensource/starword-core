#include "trie_tree.h"

#include <stack>
#include <unordered_map>
#include <vector>

#include "jwbase.h"

typedef struct TrieTree::TrieNode {
  bool is_word;
  int value;
  size_t length;
  std::unordered_map<JWChar, struct TrieNode *> children;
} TrieNode;

static TrieNode *DigTrieNode(TrieNode *current, JWChar ch) {
  auto it = current->children.find(ch);
  if (it != current->children.end()) {
    current = it->second;
  } else {
    TrieNode *node = new TrieNode();
    node->is_word = false;
    current->children.insert(std::make_pair(ch, node));
    current = node;
  }

  return current;
}

TrieTree::TrieTree() : root_(new TrieNode()) {}

TrieTree::~TrieTree() {
  std::stack<TrieNode *> clear_stack;

  for (auto it = root_->children.begin(); it != root_->children.end(); ++it) {
    clear_stack.push(it->second);
  }
  root_->children.clear();
  delete root_;

  while (!clear_stack.empty()) {
    TrieNode *node = clear_stack.top();
    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
      clear_stack.push(it->second);
    }
    node->children.clear();
    delete node;
  }
}

bool TrieTree::IsEmpty() const { return root_->children.empty(); }

void TrieTree::InsertWord(TrieNode *node, const JWCharBuffer buffer,
                          size_t length, int value, size_t base_length) {
  TrieNode *current = node;
  for (size_t i = 0; i < length; ++i) {
    current = DigTrieNode(current, buffer[i]);
  }

  if (!current->is_word) {
    current->is_word = true;
    current->value = value;
    current->length = base_length + length;
  }
}

void TrieTree::AddWord(const JWCharBuffer buffer, size_t length, int value) {
  InsertWord(root_, buffer, length, value, 0);
}

void TrieTree::AddWord(JWChar prefix, const JWCharBuffer buffer, size_t length,
                       int value) {
  InsertWord(DigTrieNode(root_, prefix), buffer, length, value, 1);
}

TrieNode *TrieTree::FindWord(const JWCharBuffer buffer, size_t start_index,
                             size_t end_index) const {
  TrieNode *current = root_;
  std::vector<TrieNode *> cached;

  for (size_t i = start_index; i < end_index; i++) {
    auto child = current->children.find(buffer[i]);
    if (child == current->children.end()) {
      break;
    }

    current = child->second;
    if (current->is_word) {
      cached.push_back(current);
    }
  }

  return cached.empty() ? nullptr : cached.back();
}

bool TrieTree::SearchWord(TrieFound &found, const JWCharBuffer buffer,
                          size_t start_index, size_t end_index) const {
  for (size_t i = start_index; i < end_index; ++i) {
    TrieNode *node = FindWord(buffer, i, end_index);
    if (node != nullptr) {
      found.value = node->value;
      found.start_index = i;
      found.stop_index = i + node->length;
      return true;
    }
  }

  return false;
}
