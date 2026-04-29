#include "star_text.h"

StarText::StarText(const StarTextOptions& options)
    : StarBase(options.ignore_case),
      method_{options.left_border, options.right_border, kNULL} {}

bool StarText::ProcessBuffer(skchar_t* buffer, size_t length) {
  TrieFound found{};
  StarStatistics statistics{};

  size_t pos = 0;
  while (pos < length) {
    if (!tree_.SearchWord(found, buffer, pos, length)) {
      break;
    }

    pos = found.stop_index;
    StarBuffer(statistics, buffer, found.start_index, found.stop_index,
               method_);
  }

  return statistics.process_count > 0;
}
