#pragma once

#include <mteval/Evaluator.h>

#include <vector>

namespace MTEval {

// Calculates BLEU and BLEU+n scores
class BLEUEvaluator : public Evaluator {
  BLEUEvaluator(const BLEUEvaluator&) = delete;
  BLEUEvaluator(BLEUEvaluator&&) = delete;
  BLEUEvaluator& operator=(const BLEUEvaluator&) = delete;
  BLEUEvaluator& operator=(BLEUEvaluator&&) = delete;

public:
  // Acceptable EvaluatorParams:
  //   "ngram"
  //     Maximum num of n-gram to be used.
  //   "smooth"
  //     Additional count for >1-grams.
  BLEUEvaluator(const std::vector<EvaluatorParam> & params);

  ~BLEUEvaluator();
  
  void prepare(const Sample& sample);

  // Statistics to be obtained:
  //   "len:hyp"
  //     <int>
  //     Number of words in hypothesis sentences.
  //   "len:ref"
  //     <int>
  //     Number of words in reference sentences.
  //   "ngram:%d:hyp"
  //     <int>
  //     Number of n-grams in the hypothesis sentence.
  //   "ngram:%d:match"
  //     <int>
  //     Number of matched n-grams between the hypothesis and the reference.
  //   "samples"
  //     <int>
  //     Number of evaluation samples.
  Statistics map(const Sample& sample) const;
  
  double integrate(const Statistics& stats) const;
  std::string getName() const;

private:
  // hyperparameters
  int ngram_;
  double smooth_;
};

} // namespace MTEval

