#include "star_base.h"

StarBase::StarBase(bool ignore_case) : tree_(ignore_case) {}
StarBase::~StarBase() {}

bool StarBase::IsEmpty() const { return tree_.IsEmpty(); };

void StarBase::AddKeyword(const skchar_t* buffer, size_t length) {
  tree_.AddWord(buffer, length);
}
