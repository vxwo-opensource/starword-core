#include "star_json.h"

#include "star_base.h"

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
  skchar_t first_char = str[str_start_index];
  size_t str_count = str_stop_index - str_start_index;

  size_t last_index = stop_index - str_count;
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

static bool TryFindKeyEnd(size_t& out_index, const skchar_t* buffer,
                          size_t start_index, size_t stop_index) {
  for (size_t i = start_index; i < stop_index; ++i) {
    skchar_t ch = buffer[i];
    if (ch == kBACKSLASH || ch == kDOUBLE_QUOTE) {
      out_index = i;
      return true;
    }
  }
  return false;
}

static bool TryFindSoftCharEnd(size_t& out_index, const skchar_t* buffer,
                               skchar_t skip, skchar_t target,
                               size_t start_index, size_t stop_index) {
  size_t pos =
      start_index + BufferSkipChar(buffer, skip, start_index, stop_index);
  if (pos >= stop_index || buffer[pos] != target) {
    return false;
  }

  out_index = pos + 1;
  return true;
}

static bool TryFindNumberEnd(size_t& out_index, const skchar_t* buffer,
                             size_t start_index, size_t stop_index) {
  size_t pos = start_index;
  while (pos < stop_index) {
    skchar_t ch = buffer[pos];
    if (!((ch >= '0' && ch <= '9') || ch == '.' || ch == '-' || ch == '+' ||
          ch == 'e' || ch == 'E')) {
      break;
    }

    ++pos;
  }

  if (pos <= start_index) {
    return false;
  }

  out_index = pos;
  return true;
}

static bool TryFindSymbolEnd(size_t& out_index, const skchar_t* buffer,
                             size_t start_index, size_t stop_index) {
  size_t size = stop_index - start_index;
  if (size < 4) {
    return false;
  }

  int length = 0;
  skchar_t first_char = buffer[start_index];

  if (first_char == 'n') {  // Test the symbol null
    if (buffer[start_index + 1] == 'u' && buffer[start_index + 2] == 'l' &&
        buffer[start_index + 3] == 'l') {
      length = 4;
    }
  } else if (first_char == 't') {  // Test the symbol true
    if (buffer[start_index + 1] == 'r' && buffer[start_index + 2] == 'u' &&
        buffer[start_index + 3] == 'e') {
      length = 4;
    }
  } else if (size > 4 && first_char == 'f') {  // Test the symbol false
    if (buffer[start_index + 1] == 'a' && buffer[start_index + 2] == 'l' &&
        buffer[start_index + 3] == 's' && buffer[start_index + 4] == 'e') {
      length = 5;
    }
  }

  if (length < 1) {
    return false;
  }

  out_index = length;
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
    if (!tree_.SearchWord(found, buffer, pos, length)) {
      break;
    }

    if (found.stop_index >= length) {
      break;
    }

    // Find an exact matching for the KEY suffix
    size_t begin_index = 0;
    if (!TryFindKeyEnd(begin_index, buffer, found.stop_index, length)) {
      pos = found.stop_index;
      continue;
    } else if (begin_index != found.stop_index) {
      pos = begin_index;
      continue;
    }

    // Find the symbol `double quote`
    size_t end_index = 0;
    if (!TryFindSoftCharEnd(end_index, buffer, kBACKSLASH, kDOUBLE_QUOTE,
                            begin_index, length)) {
      pos = begin_index;
      continue;
    }

    // Find the symbol `colon`
    size_t colon_index = 0;
    if (!TryFindSoftCharEnd(colon_index, buffer, kBLANK, kCOLON, end_index,
                            length)) {
      pos = end_index;
      continue;
    }

    pos = ProcessComplexValue(context, buffer, colon_index, length, true);
  }

  return context.process_count > 0;
}

size_t StarJson::ProcessComplexValue(StarContext& context, skchar_t* buffer,
                                     size_t start_index, size_t end_index,
                                     bool enter_array) {
  // Skip blank
  size_t prefix_begin_index =
      start_index + BufferSkipChar(buffer, kBLANK, start_index, end_index);
  if (prefix_begin_index >= end_index) {
    return prefix_begin_index;
  }

  // Test value is symbol
  size_t symbol_end_index = 0;
  if (TryFindSymbolEnd(symbol_end_index, buffer, prefix_begin_index,
                       end_index)) {
    return symbol_end_index;
  }

  skchar_t first_char = buffer[prefix_begin_index];

  // Test value is object
  if (first_char == kLEFT_BRACE) {
    return prefix_begin_index + 1;
  }

  if (first_char != kLEFT_BRACKET) {
    return ProcessSimpleValue(context, buffer, prefix_begin_index, end_index);
  } else if (!enter_array) {
    return prefix_begin_index + 1;
  } else {
    size_t pos = prefix_begin_index + 1;
    while (pos < end_index) {
      size_t next_end_index =
          ProcessComplexValue(context, buffer, pos, end_index, false);

      // Skip blank
      next_end_index +=
          BufferSkipChar(buffer, kBLANK, next_end_index, end_index);
      if (next_end_index >= end_index) {
        pos = next_end_index;
        break;
      }

      pos = next_end_index + 1;

      // Find the symbol `comma`
      skchar_t stop_char = buffer[next_end_index];
      if (stop_char != kCOMMA) {
        break;
      }
    }
    return pos;
  }
}

size_t StarJson::ProcessSimpleValue(StarContext& context, skchar_t* buffer,
                                    size_t start_index, size_t end_index) {
  // Find the number
  size_t number_end_index = 0;
  if (TryFindNumberEnd(number_end_index, buffer, start_index, end_index)) {
    return number_end_index;
  }

  // Find the symbol `double quote`
  size_t prefix_end_index = 0;
  if (!TryFindSoftCharEnd(prefix_end_index, buffer, kBACKSLASH, kDOUBLE_QUOTE,
                          start_index, end_index)) {
    return start_index;
  }

  size_t prefix_length = prefix_end_index - start_index;

  // Find the the VALUE suffix
  size_t suffix_begin_index = 0;
  size_t pos = prefix_end_index;
  while (pos < end_index) {
    if (!BufferIndexOfStr(suffix_begin_index, buffer, pos, end_index, buffer,
                          start_index, prefix_end_index)) {
      break;
    }

    // Skip escape characters for an odd number of double quotes
    size_t number_backslash = BufferForwardSkipChar(
        buffer, kBACKSLASH, prefix_end_index, suffix_begin_index - 1);
    if (number_backslash % 2 == 0) {
      break;
    }

    pos = suffix_begin_index + prefix_length;
  }

  // Skip no VALUE suffix
  if (suffix_begin_index < 0) {
    return start_index;
  }

  if (suffix_begin_index > prefix_end_index) {
    StarBuffer(context, buffer, prefix_end_index, suffix_begin_index);
  }

  return suffix_begin_index + prefix_length;
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
