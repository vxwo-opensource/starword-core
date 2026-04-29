#ifndef SKC_STAR_JSON_H_
#define SKC_STAR_JSON_H_

#include "star_base.h"

struct StarJsonOptions {
  bool ignore_case;
  StarMethod method;
};

struct StarJsonLocal;
class StarJson : public StarBase {
 public:
  StarJson(const StarJsonOptions& options);
  virtual ~StarJson();

  void AddWord(const skchar_t* buffer, size_t length) override;
  void AddWord(const skchar_t* buffer, size_t length, const StarMethod& method);

  bool ProcessBuffer(skchar_t* buffer, size_t length) override;

 private:
  StarMethod method_;
  StarJsonLocal* local_;
};

#endif
