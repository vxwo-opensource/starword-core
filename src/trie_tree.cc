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
  size_t length;
  std::unordered_map<skchar_t, struct TrieNode *> children;
};

struct TrieContext {
  size_t skip;
  std::vector<TrieNode *> nodes;
};

static TrieNode *DigTrieNode(TrieNode *current, skchar_t ch) {
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
  TrieNode *current = node;
  for (size_t i = 0; i < length; ++i) {
    current = DigTrieNode(current, transform(ignore_case_, buffer[i]));
  }

  if (!current->is_word) {
    current->is_word = true;
    current->length = length + extra_length;
  }
}

void TrieTree::AddWord(const skchar_t *buffer, size_t length) {
  InsertWord(root_, buffer, length, 0);
}

void TrieTree::AddWord(skchar_t prefix, const skchar_t *buffer, size_t length) {
  InsertWord(DigTrieNode(root_, prefix), buffer, length, 1);
}

TrieNode *TrieTree::FindWord(TrieContext &context, const skchar_t *buffer,
                             size_t start_index, size_t end_index) const {
  TrieNode *current = root_;

  context.skip = 0;
  context.nodes.clear();
  for (size_t i = start_index; i < end_index; i++) {
    auto child = current->children.find(transform(ignore_case_, buffer[i]));
    if (child == current->children.end()) {
      break;
    }

    ++context.skip;
    current = child->second;
    if (current->is_word) {
      context.nodes.push_back(current);
    }
  }

  return context.nodes.empty() ? nullptr : context.nodes.back();
}

bool TrieTree::SearchWord(TrieFound &found, const skchar_t *buffer,
                          size_t start_index, size_t end_index) const {
  TrieContext context;
  size_t pos = start_index;

  while (pos < end_index) {
    TrieNode *node = FindWord(context, buffer, pos, end_index);
    if (node != nullptr) {
      found.start_index = pos;
      found.stop_index = pos + node->length;
      return true;
    }

    if (context.skip < 2) {
      ++pos;
    } else {
      pos += context.skip;
    }
  }

  return false;
}
