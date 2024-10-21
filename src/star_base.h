#ifndef SKC_STAR_BASE_H_
#define SKC_STAR_BASE_H_

#include "trie_tree.h"

const skchar_t kSTAR = '*';

struct StarOptions {
  bool ignore_case;
  size_t left_border;
  size_t right_border;
};

class StarBase {
 public:
  virtual ~StarBase() = 0;

  bool IsEmpty() const;
  void FinishAdd();

  virtual void AddWord(const skchar_t* buffer, size_t length);
  virtual bool ProcessBuffer(skchar_t* buffer, size_t length) = 0;

 protected:
  struct StarContext {
    size_t process_count;
    size_t character_count;
  };

  TrieTree tree_;
  StarOptions options_;

  StarBase(const StarOptions& options);

  virtual void StarBuffer(StarContext& context, skchar_t* buffer,
                          size_t start_index, size_t end_index);
};

#endif
