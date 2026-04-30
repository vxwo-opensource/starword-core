#include "trie_tree.h"

#include <stack>
#include <unordered_map>
#include <unordered_set>

struct TrieNode {
  TrieNode() = default;
  TrieNode(const TrieNode&) = delete;
  TrieNode& operator=(const TrieNode&) = delete;
  TrieNode(TrieNode&&) = delete;
  TrieNode& operator=(TrieNode&&) = delete;

  bool is_word = false;
  uint16_t skip = 0;
  uint16_t length = 0;
  const void* payload = nullptr;
  std::unordered_map<skchar_t, struct TrieNode*> children;
};

static TrieNode* DigTrieNode(TrieNode* cursor, skchar_t ch, bool ignore_case) {
  TrieNode* child = nullptr;
  auto it = cursor->children.find(ch);
  if (it != cursor->children.end()) {
    child = it->second;
  } else {
    child = new TrieNode();
    child->is_word = false;
    cursor->children.insert(std::make_pair(ch, child));
  }

  if (ignore_case && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))) {
    skchar_t other = ch ^ 0x20;
    if (cursor->children.find(other) == cursor->children.end()) {
      cursor->children.insert(std::make_pair(other, child));
    }
  }

  return child;
}

TrieTree::TrieTree(bool ignore_case)
    : root_(new TrieNode()), ignore_case_(ignore_case) {}

TrieTree::~TrieTree() {
  std::stack<TrieNode*> clear_stack;
  std::unordered_set<const TrieNode*> seen;

  for (auto it = root_->children.begin(); it != root_->children.end(); ++it) {
    TrieNode* node = it->second;
    if (seen.insert(node).second) {
      clear_stack.push(node);
    }
  }
  root_->children.clear();

  while (!clear_stack.empty()) {
    TrieNode* node = clear_stack.top();
    clear_stack.pop();

    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
      TrieNode* child = it->second;
      if (seen.insert(child).second) {
        clear_stack.push(child);
      }
    }
    node->children.clear();
    delete node;
  }

  seen.clear();
  delete root_;
}

bool TrieTree::IsEmpty() const { return root_->children.empty(); }

void TrieTree::InsertWord(TrieNode* node, const skchar_t* buffer, size_t length,
                          size_t extra_length, const void* payload) {
  TrieNode* cursor = node;
  for (size_t i = 0; i < length; ++i) {
    cursor = DigTrieNode(cursor, buffer[i], ignore_case_);
  }

  if (!cursor->is_word) {
    cursor->is_word = true;
    cursor->length = length + extra_length;
    cursor->payload = payload;
  }
}

void TrieTree::AddWord(const skchar_t* buffer, size_t length,
                       const void* payload) {
  InsertWord(root_, buffer, length, 0, payload);
}

void TrieTree::AddWord(skchar_t prefix, const skchar_t* buffer, size_t length,
                       const void* payload) {
  InsertWord(DigTrieNode(root_, prefix, ignore_case_), buffer, length, 1,
             payload);
}

void TrieTree::FinishAdd() {
  std::stack<TrieNode*> skip_stack;
  std::unordered_set<const TrieNode*> seen;

  for (auto it = root_->children.begin(); it != root_->children.end(); ++it) {
    TrieNode* node = it->second;
    if (seen.insert(node).second) {
      node->skip = 1;
      skip_stack.push(node);
    }
  }

  while (!skip_stack.empty()) {
    TrieNode* node = skip_stack.top();
    skip_stack.pop();

    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
      TrieNode* child = it->second;
      if (!seen.insert(child).second) {
        continue;
      }

      if (root_->children.find(it->first) != root_->children.end()) {
        child->skip = 1;
      } else {
        child->skip = node->skip + 1;
      }

      skip_stack.push(child);
    }
  }
}

bool TrieTree::SearchWord(TrieFound& found, const skchar_t* buffer,
                          size_t start_index, size_t stop_index) const {
  TrieNode* cursor = root_;
  TrieNode* last_match = nullptr;
  size_t pos = start_index, save_pos = pos, save_skip = 1;

  while (pos < stop_index) {
    auto child = cursor->children.find(buffer[pos]);
    if (child != cursor->children.end()) {
      ++pos;
      cursor = child->second;

      save_skip = cursor->skip;
      if (cursor->is_word) {
        last_match = cursor;
      }
    } else {
      if (last_match) {
        break;
      }

      cursor = root_;
      pos = save_pos + save_skip;
      save_pos = pos;
      save_skip = 1;
    }
  }

  if (!last_match) {
    return false;
  }

  found.start_index = save_pos;
  found.stop_index = save_pos + last_match->length;
  found.payload = last_match->payload;
  return true;
}
