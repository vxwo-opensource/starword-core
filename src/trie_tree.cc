#include "trie_tree.h"

#include <stack>
#include <unordered_map>
#include <vector>

static inline skchar_t transform(bool ignore, skchar_t value) {
  return !ignore ? value
                 : ((value >= 'A' && value <= 'Z') ? (value + 0x20) : value);
}

struct TrieNode {
  bool is_word;
  size_t skip;
  size_t length;
  std::unordered_map<skchar_t, struct TrieNode *> children;
};

static TrieNode *DigTrieNode(TrieNode *cursor, skchar_t ch) {
  auto it = cursor->children.find(ch);
  if (it != cursor->children.end()) {
    cursor = it->second;
  } else {
    TrieNode *node = new TrieNode();
    node->is_word = false;
    cursor->children.insert(std::make_pair(ch, node));
    cursor = node;
  }

  return cursor;
}

TrieTree::TrieTree(bool ignore_case)
    : root_(new TrieNode()), ignore_case_(ignore_case) {}

TrieTree::~TrieTree() {
  std::stack<TrieNode *> clear_stack;

  for (auto it = root_->children.begin(); it != root_->children.end(); ++it) {
    clear_stack.push(it->second);
  }
  root_->children.clear();

  while (!clear_stack.empty()) {
    TrieNode *node = clear_stack.top();
    clear_stack.pop();

    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
      clear_stack.push(it->second);
    }
    node->children.clear();
    delete node;
  }

  delete root_;
}

bool TrieTree::IsEmpty() const { return root_->children.empty(); }

void TrieTree::InsertWord(TrieNode *node, const skchar_t *buffer, size_t length,
                          size_t extra_length) {
  TrieNode *cursor = node;
  for (size_t i = 0; i < length; ++i) {
    cursor = DigTrieNode(cursor, transform(ignore_case_, buffer[i]));
  }

  if (!cursor->is_word) {
    cursor->is_word = true;
    cursor->length = length + extra_length;
  }
}

void TrieTree::AddWord(const skchar_t *buffer, size_t length) {
  InsertWord(root_, buffer, length, 0);
}

void TrieTree::AddWord(skchar_t prefix, const skchar_t *buffer, size_t length) {
  InsertWord(DigTrieNode(root_, prefix), buffer, length, 1);
}

void TrieTree::FinishAdd() {
  std::stack<TrieNode *> skip_stack;

  for (auto it = root_->children.begin(); it != root_->children.end(); ++it) {
    TrieNode *node = it->second;
    node->skip = 1;
    skip_stack.push(node);
  }

  while (!skip_stack.empty()) {
    TrieNode *node = skip_stack.top();
    skip_stack.pop();

    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
      TrieNode *child = it->second;

      if (root_->children.find(it->first) != root_->children.end()) {
        child->skip = 1;
      } else {
        child->skip = node->skip + 1;
      }

      skip_stack.push(child);
    }
  }
}

bool TrieTree::SearchWord(TrieFound &found, const skchar_t *buffer,
                          size_t start_index, size_t stop_index) const {
  bool found_word = false;
  TrieNode *cursor = root_;
  std::vector<TrieNode *> save_nodes;
  size_t pos = start_index, save_skip = 0, save_pos = pos;

  while (pos < stop_index) {
    auto child = cursor->children.find(transform(ignore_case_, buffer[pos]));
    if (child != cursor->children.end()) {
      ++pos;
      cursor = child->second;

      save_skip = cursor->skip;
      if (cursor->is_word) {
        found_word = true;
        save_nodes.push_back(cursor);
      }
    } else {
      if (found_word) {
        break;
      }

      cursor = root_;
      pos = save_pos + (save_skip < 2 ? 1 : save_skip);
      save_skip = 0;
      save_pos = pos;
    }
  }

  if (!found_word) {
    return false;
  }

  TrieNode *node = save_nodes.back();
  found.start_index = save_pos;
  found.stop_index = save_pos + node->length;
  return true;
}
