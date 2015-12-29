#pragma once

#include <mteval/Evaluator.h>

#include <vector>

namespace MTEval {

// Calculates NIST and scores
class NISTEvaluator : public Evaluator {
  NISTEvaluator(const NISTEvaluator&) = delete;
  NISTEvaluator(NISTEvaluator&&) = delete;
  NISTEvaluator& operator=(const NISTEvaluator&) = delete;
  NISTEvaluator& operator=(NISTEvaluator&&) = delete;

public:
  // Acceptable EvaluatorParams:
  //   "ngram"
  //     Maximum num of n-gram to be used.
  NISTEvaluator(const std::vector<EvaluatorParam> & params);

  ~NISTEvaluator();
  
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
  //     <real>
  //     Cumulative weighted n-gram matches between the hypothesis and the
  //     reference.
  //   "samples"
  //     <int>
  //     Number of evaluation samples.
  Statistics map(const Sample& sample) const;
  
  double integrate(const Statistics& stats) const;
  std::string getName() const;

private:
  // inner statistics
  std::map<Sentence, int> freq_;

  // hyperparameters
  int ngram_;
};

} // namespace MTEval

