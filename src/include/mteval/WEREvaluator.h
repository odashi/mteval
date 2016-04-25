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
  //   "substitute"
  //     weight of substituting ref/hyp words
  //   "insert"
  //     weight of inserting a hyp word
  //   "delete"
  //     weight of deletion a hyp word
  WEREvaluator(const std::vector<EvaluatorParam> & params);

  ~WEREvaluator();
  
  void prepare(const Sample& sample);

  // Statistics to be obtained:
  //   "distance"
  //     <int>
  //     Cumulative Levenshtein distance for each evaluation sample.
  //   "samples"
  //     <int>
  //     Number of evaluation samples.
  //   "score"
  //     <real>
  //     Cumulative WER score for each evaluation sample.
  Statistics map(const Sample& sample) const;
  
  double integrate(const Statistics& stats) const;
  std::string getName() const;

private:
  // hyperparamaters
  double w_sub_;
  double w_ins_;
  double w_del_;
};

} // namespace MTEval

