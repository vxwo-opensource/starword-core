#include "star_base.h"

StarBase::StarBase(const StarOptions& options)
    : tree_(options.ignore_case), options_(options) {}
StarBase::~StarBase() {}

bool StarBase::IsEmpty() const { return tree_.IsEmpty(); }

void StarBase::AddKeyword(const skchar_t* buffer, size_t length) {
  tree_.AddWord(buffer, length);
}

void StarBase::StarBuffer(StarContext& context, skchar_t* buffer,
                          size_t start_index, size_t end_index) {
  ssize_t effective = end_index - start_index;
  if (effective < 1) {
    return;
  }

  ssize_t left_border = options_.left_border;
  ssize_t right_border = options_.right_border;
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
