#ifndef SKC_STAR_BASE_H_
#define SKC_STAR_BASE_H_

#include "trie_tree.h"

const skchar_t kNULL = 0x0;
const skchar_t kSTAR = '*';

struct StarMethod {
  size_t left_border;
  size_t right_border;
  skchar_t right_border_char;
};

struct StarStatistics {
  size_t process_count;
  size_t character_total;
};

class StarBase {
 public:
  virtual ~StarBase() = 0;

  bool IsEmpty() const;
  void FinishAdd();

  virtual void AddWord(const skchar_t* buffer, size_t length);
  virtual bool ProcessBuffer(skchar_t* buffer, size_t length) = 0;

  static bool ProcessBuffer(skchar_t* buffer, size_t length,
                            const StarMethod& method);

 protected:
  TrieTree tree_;

  StarBase(bool ignore_case);
  StarBase(const StarBase&) = delete;
  StarBase& operator=(const StarBase&) = delete;
  StarBase(StarBase&&) = delete;
  StarBase& operator=(StarBase&&) = delete;

  static bool IsMethodEqual(const StarMethod& a, const StarMethod& b);
  static void StarPartialBuffer(StarStatistics& statistics, skchar_t* buffer,
                                size_t start_index, size_t stop_index,
                                const StarMethod& method);
};

#endif
