#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "../src/star_text.h"

constexpr int kPrefixWordCount = 200000;
constexpr int kRandomWordCount = 200000;
constexpr int kTextLength = 100000;
constexpr int kInsertCount = 400;
constexpr int kIterations = 100;
constexpr unsigned int kRandomSeed = 1;
constexpr size_t kLeftBorder = 1;
constexpr size_t kRightBorder = 1;

std::u16string genRandom(int length) {
  std::u16string s;
  s.reserve(length);
  for (int i = 0; i < length; i++) {
    s += static_cast<char16_t>('a' + rand() % 26);
  }
  return s;
}

std::u16string genKeyword(int index) {
  std::u16string keyword(u"keyword_");
  std::string number = std::to_string(index);
  for (char c : number) {
    keyword += static_cast<char16_t>(c);
  }
  return keyword;
}

double toMilliseconds(const std::chrono::steady_clock::duration& duration) {
  return std::chrono::duration<double, std::milli>(duration).count();
}

int main() {
  srand(kRandomSeed);

  auto build_start = std::chrono::steady_clock::now();

  StarTextOptions options{true, kLeftBorder, kRightBorder};
  StarText engine(options);

  for (int i = 0; i < kPrefixWordCount; i++) {
    std::u16string keyword = genKeyword(i);
    engine.AddWord((skchar_t*)keyword.data(), keyword.size());
  }
  for (int i = 0; i < kRandomWordCount; i++) {
    std::u16string keyword = genRandom(5 + rand() % 10);
    engine.AddWord((skchar_t*)keyword.data(), keyword.size());
  }
  engine.FinishAdd();

  auto build_end = std::chrono::steady_clock::now();
  fprintf(stderr, "[bench] build: %.3f ms, keywords=%d\n",
          toMilliseconds(build_end - build_start),
          kPrefixWordCount + kRandomWordCount);

  std::u16string base;
  base.reserve(kTextLength);
  for (int i = 0; i < kTextLength; i++) {
    base += static_cast<char16_t>('a' + (i % 26));
  }

  std::u16string hit = base;
  for (int i = 0; i < kInsertCount; i++) {
    std::u16string keyword = genKeyword(i * 1000);
    size_t pos = 100 + i * (kTextLength / kInsertCount);
    hit.replace(pos, keyword.size(), keyword);
  }

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < kIterations; i++) {
    engine.ProcessBuffer((skchar_t*)base.data(), base.size());
  }
  auto end = std::chrono::steady_clock::now();
  fprintf(stderr, "[bench] no_match: %.3f ms/iter, %d iter\n",
          toMilliseconds(end - start) / kIterations, kIterations);

  std::u16string buffer = hit;
  start = std::chrono::steady_clock::now();
  for (int i = 0; i < kIterations; i++) {
    buffer = hit;
    engine.ProcessBuffer((skchar_t*)buffer.data(), buffer.size());
  }
  end = std::chrono::steady_clock::now();
  fprintf(stderr, "[bench] match: %.3f ms/iter, %d iter\n",
          toMilliseconds(end - start) / kIterations, kIterations);

  return 0;
}
