#include "star_text.h"

StarText::StarText(const StarOptions& options) : StarBase(options) {}

bool StarText::ProcessBuffer(skchar_t* buffer, size_t length) {
  TrieFound found{};
  StarContext context{};

  size_t pos = 0;
  while (pos < length) {
    if (!tree_.SearchWord(found, buffer, pos, length)) {
      break;
    }

    pos = found.stop_index;
    StarBuffer(context, buffer, found.start_index, found.stop_index);
  }

  return context.process_count > 0;
}
