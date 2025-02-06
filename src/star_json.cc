#include "star_json.h"

const skchar_t kBLANK = ' ';
const skchar_t kCOLON = ':';
const skchar_t kCOMMA = ',';
const skchar_t kBACKSLASH = '\\';
const skchar_t kDOUBLE_QUOTE = '\"';
const skchar_t kLEFT_BRACE = '{';
const skchar_t kLEFT_BRACKET = '[';

static size_t BufferSkipChar(const skchar_t* buffer, skchar_t ch,
                             size_t start_index, size_t stop_index) {
  size_t pos = start_index;
  while (pos < stop_index && buffer[pos] == ch) {
    ++pos;
  }
  return pos - start_index;
}

static size_t BufferForwardSkipChar(const skchar_t* buffer, skchar_t ch,
                                    size_t start_index, size_t stop_index) {
  size_t pos = stop_index;
  while (pos >= start_index && buffer[pos] == ch) {
    --pos;
  }
  return stop_index - pos;
}

static bool BufferIndexOfStr(size_t& out_index, const skchar_t* buffer,
                             size_t start_index, size_t stop_index,
                             const skchar_t* str, size_t str_start_index,
                             size_t str_stop_index) {
  const size_t str_count = str_stop_index - str_start_index;

  const skchar_t first_char = str[str_start_index];
  const size_t last_index = stop_index - str_count;
  for (size_t i = start_index; i <= last_index; i++) {
    if (buffer[i] != first_char) {
      while (++i <= last_index && buffer[i] != first_char);
    }

    if (i <= last_index) {
      size_t j = i + 1;
      size_t end = j + str_count - 1;
      for (size_t k = str_start_index + 1; j < end && buffer[j] == str[k];
           j++, k++);
      if (j == end) {
        out_index = i;
        return true;
      }
    }
  }

  return false;
}

static bool SmartSearchTargetChar(size_t& out_index, const skchar_t* buffer,
                                  skchar_t skip, skchar_t target,
                                  size_t start_index, size_t stop_index) {
  if (start_index >= stop_index) {
    return false;
  }

  size_t pos = start_index;
  if (buffer[pos] == skip) {
    pos += BufferSkipChar(buffer, skip, pos, stop_index);
    if (pos >= stop_index) {
      return false;
    }
  }

  if (buffer[pos] != target) {
    return false;
  }

  out_index = pos + 1;
  return true;
}

StarJson::StarJson(const StarOptions& options) : StarBase(options) {}

void StarJson::AddWord(const skchar_t* buffer, size_t length) {
  tree_.AddWord(kDOUBLE_QUOTE, buffer, length);
}

bool StarJson::ProcessBuffer(skchar_t* buffer, size_t length) {
  TrieFound found{};
  StarContext context{};

  size_t pos = 0;
  while (pos < length) {
    // Find the KEY prefix
    if (!tree_.SearchWord(found, buffer, pos, length) ||
        found.stop_index >= length) {
      break;
    }

    // Find the KEY suffix
    size_t key_next_index = 0;
    if (!SmartSearchTargetChar(key_next_index, buffer, kBACKSLASH,
                               kDOUBLE_QUOTE, found.stop_index, length)) {
      pos = found.stop_index;
      continue;
    }

    // Find the KEY/VALUE separator
    size_t colon_next_index = 0;
    if (!SmartSearchTargetChar(colon_next_index, buffer, kBLANK, kCOLON,
                               key_next_index, length)) {
      pos = key_next_index;
      continue;
    }

    pos = ProcessComplexValue(context, buffer, colon_next_index, length, true);
  }

  return context.process_count > 0;
}

size_t StarJson::ProcessComplexValue(StarContext& context, skchar_t* buffer,
                                     size_t start_index, size_t stop_index,
                                     bool enter_array) {
  // Skip blank
  const size_t begin_index =
      start_index + BufferSkipChar(buffer, kBLANK, start_index, stop_index);
  if (begin_index >= stop_index) {
    return begin_index;
  }

  const skchar_t first_char = buffer[begin_index];

  // Skip OBJECT type
  if (first_char == kLEFT_BRACE) {
    return begin_index + 1;
  }

  // Process simple types
  if (first_char != kLEFT_BRACKET) {
    return ProcessSimpleValue(context, buffer, begin_index, stop_index);
  }

  // Skipping ARRAY nesting
  if (!enter_array) {
    return begin_index + 1;
  }

  size_t pos = begin_index + 1;
  while (pos < stop_index) {
    size_t next_index =
        ProcessComplexValue(context, buffer, pos, stop_index, false);

    // Skip blank
    next_index += BufferSkipChar(buffer, kBLANK, next_index, stop_index);
    if (next_index >= stop_index) {
      pos = next_index;
      break;
    }

    pos = next_index + 1;

    // Find the ARRAY separator
    if (buffer[next_index] != kCOMMA) {
      break;
    }
  }
  return pos;
}

size_t StarJson::ProcessSimpleValue(StarContext& context, skchar_t* buffer,
                                    size_t start_index, size_t stop_index) {
  // Skip NUMBER and SYMBOL
  size_t pos = start_index;
  while (pos < stop_index) {
    const skchar_t ch = buffer[pos];
    if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
          (ch >= '0' && ch <= '9') || ch == '.' || ch == '-' || ch == '+')) {
      break;
    }

    ++pos;
  }
  if (pos > start_index) {
    return pos;
  }

  // Find the TEXT prefix
  size_t value_begin_index = 0;
  if (!SmartSearchTargetChar(value_begin_index, buffer, kBACKSLASH,
                             kDOUBLE_QUOTE, start_index, stop_index)) {
    return start_index;
  }

  size_t value_end_index = 0;
  const size_t wrapper_length = value_begin_index - start_index;

  pos = value_begin_index;
  while (pos < stop_index) {
    if (!BufferIndexOfStr(value_end_index, buffer, pos, stop_index, buffer,
                          start_index, value_begin_index)) {
      break;
    }

    // Skip escape characters for an odd number of double quotes
    const size_t backslash_count = BufferForwardSkipChar(
        buffer, kBACKSLASH, value_begin_index, value_end_index - 1);
    if (backslash_count % 2 == 0) {
      break;
    }

    pos = value_end_index + wrapper_length;
  }

  // Skip no TEXT suffix
  if (value_end_index == 0) {
    return start_index;
  }

  if (value_end_index > value_begin_index) {
    StarBuffer(context, buffer, value_begin_index, value_end_index);
  }

  return value_end_index + wrapper_length;
}

void StarJson::StarBuffer(StarContext& context, skchar_t* buffer,
                          size_t start_index, size_t stop_index) {
  if (start_index > 0 && buffer[start_index - 1] == kBACKSLASH &&
      BufferForwardSkipChar(buffer, kBACKSLASH, 0, start_index - 1) % 2 != 0) {
    start_index += 1;
  }

  if (stop_index > 0 && buffer[stop_index - 1] == kBACKSLASH &&
      BufferForwardSkipChar(buffer, kBACKSLASH, start_index, stop_index - 1) %
              2 !=
          0) {
    stop_index -= 1;
  }

  StarBase::StarBuffer(context, buffer, start_index, stop_index);
}
