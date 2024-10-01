#include "star_text.h"

StarText::StarText(const StarOptions& options) : StarBase(options) {}

bool StarText::ProcessBuffer(skchar_t* buffer, size_t length) {
  size_t index = 0;
  TrieFound found{0, 0};
  StarContext context{0, 0};
  while (index < length) {
    // Find the keyword
    if (!tree_.SearchWord(found, buffer, index, length)) {
      break;
    }

    StarBuffer(context, buffer, found.start_index, found.stop_index);
    index = found.stop_index;
  }

  return context.count > 0;
}
