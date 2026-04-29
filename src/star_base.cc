#include "star_base.h"

static inline bool IsUtf16Surrogate(skchar_t v) {
  return v >= 0xD800 && v <= 0xDFFF;
}

StarBase::StarBase(bool ignore_case) : tree_(ignore_case) {}
StarBase::~StarBase() {}

bool StarBase::IsEmpty() const { return tree_.IsEmpty(); }

void StarBase::FinishAdd() { tree_.FinishAdd(); }

void StarBase::AddWord(const skchar_t* buffer, size_t length) {
  tree_.AddWord(buffer, length, nullptr);
}

bool StarBase::IsEqMethod(const StarMethod& a, const StarMethod& b) {
  return a.left_border == b.left_border && a.right_border == b.right_border &&
         a.right_border_char == b.right_border_char;
}

void StarBase::StarBuffer(StarStatistics& statistics, skchar_t* buffer,
                          size_t start_index, size_t stop_index,
                          const StarMethod& method) {
  if (method.right_border_char != kNULL) {
    bool found = false;
    size_t op = stop_index;
    while (op > start_index) {
      op -= 1;
      if (buffer[op] == method.right_border_char) {
        found = true;
        break;
      }
    }
    if (found) {
      stop_index = op;
    }
  }

  size_t effective = stop_index - start_index;
  if (effective < 1) {
    return;
  }

  size_t left_border = method.left_border;
  if (left_border >= effective) {
    left_border = effective - 1;
  }

#ifdef SKC_USE_UTF16
  if (left_border > 0 && IsUtf16Surrogate(buffer[start_index + left_border])) {
    left_border += 1;
  }
#endif

  size_t right_border = method.right_border;
  if (right_border >= effective - left_border) {
    right_border = 0;
  }

#ifdef SKC_USE_UTF16
  if (right_border > 0 && IsUtf16Surrogate(buffer[stop_index - right_border])) {
    right_border += 1;
  }
#endif

  if (left_border > 0) {
    start_index += left_border;
  }
  if (right_border > 0) {
    stop_index -= right_border;
  }

  effective = stop_index - start_index;
  if (effective < 1) {
    return;
  }

  statistics.process_count += 1;
  statistics.character_total += effective;

  size_t pos = start_index;
  while (pos + 4 < stop_index) {
    buffer[pos] = kSTAR;
    buffer[pos + 1] = kSTAR;
    buffer[pos + 2] = kSTAR;
    buffer[pos + 3] = kSTAR;
    pos += 4;
  }
  while (pos < stop_index) {
    buffer[pos++] = kSTAR;
  }
}
