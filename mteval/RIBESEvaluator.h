#pragma once

#include <mteval/Evaluator.h>

#include <vector>

namespace MTEval {

// Calculates RIBES score
class RIBESEvaluator : public Evaluator {
  RIBESEvaluator(const RIBESEvaluator&) = delete;
  RIBESEvaluator(RIBESEvaluator&&) = delete;
  RIBESEvaluator& operator=(const RIBESEvaluator&) = delete;
  RIBESEvaluator& operator=(RIBESEvaluator&&) = delete;

public:
  // Acceptable EvaluatorParams:
  //   "alpha"
  //     Weight of unigram precision.
  //   "beta"
  //     Weight of brevity penalty.
  RIBESEvaluator(const std::vector<EvaluatorParam> & params);

  ~RIBESEvaluator();
  
  void prepare(const Sample& sample);

  // Statistics to be obtained:
  //   "brevity"
  //     <real>
  //     Cumulative brevity penalty for each evaluation sample.
  //   "nkt"
  //     <real>
  //     Cumulative Kendall's tau for each evaluation sample.
  //   "prec"
  //     <real>
  //     Cumulative unigram precision for each evaluation sample.
  //   "samples"
  //     <int>
  //     Number of evaluation samples.
  //   "score"
  //     <real>
  //     Cumulative RIBES score for each evaluation sample.
  Statistics map(const Sample& sample) const;
  
  double integrate(const Statistics& stats) const;
  std::string getName() const;

private:
  // hyperparameters
  double alpha_;
  double beta_;
};

} // namespace MTEval

