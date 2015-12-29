#pragma once

#include <mteval/Evaluator.h>

#include <vector>

namespace MTEval {

// Calculates WER and WER+n scores
class WEREvaluator : public Evaluator {
  WEREvaluator(const WEREvaluator&) = delete;
  WEREvaluator(WEREvaluator&&) = delete;
  WEREvaluator& operator=(const WEREvaluator&) = delete;
  WEREvaluator& operator=(WEREvaluator&&) = delete;

public:
  // Acceptable EvaluatorParams:
  //   none
  WEREvaluator(const std::vector<EvaluatorParam> & params);

  ~WEREvaluator();
  
  void prepare(const Sample& sample);

  // Statistics to be obtained:
  //   "samples"
  //     <int>
  //     Number of evaluation samples.
  //   "score"
  //     <real>
  //     Cumulative WER score for each evaluation sample.
  Statistics map(const Sample& sample) const;
  
  double integrate(const Statistics& stats) const;
  std::string getName() const;
};

} // namespace MTEval

