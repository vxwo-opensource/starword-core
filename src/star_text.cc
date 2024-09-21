#include "star_text.h"

StarText::StarText(bool ignore_case, size_t border)
    : StarBase(ignore_case), border_(border) {}

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

void StarText::StarBuffer(StarContext& context, skchar_t* buffer,
                          size_t start_index, size_t end_index) {
  ssize_t effective = end_index - start_index;
  if (effective < 1) {
    return;
  }

  ssize_t left_border = border_;
  ssize_t right_border = border_;
  if (left_border >= effective) {
    left_border = effective - 1;
  }
  if (left_border < 0) {
    left_border = 0;
  }
  if (right_border >= effective - left_border) {
    right_border = 0;
  }
  if (left_border > 0) {
    start_index += left_border;
  }
  if (right_border > 0) {
    end_index -= right_border;
  }

  if (start_index < end_index) {
    context.count += 1;
    context.char_length += end_index - start_index;

    while (start_index < end_index) {
      buffer[start_index++] = kSTAR;
    }
  }
}
