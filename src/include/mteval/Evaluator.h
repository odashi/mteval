#pragma once

#include <mteval/utils.h>
#include <mteval/Statistics.h>

#include <string>
#include <vector>

namespace MTEval {

// This structure specifies a parameter for the Evaluator.
// Allowed parameters are depended on each Evaluator, and they should be
// specified in each Evaluator's header file.
struct EvaluatorParam {
  // Parameter name used to recognize parameter type.
  // If the Evaluator receives a parameter with unknown name, it should throw
  // the exception.
  std::string name;

  // Parameter values.
  // Only one field is used, but there is multiple field due to
  // EvaluatorFactory implementation.
  int int_val;
  double real_val;
};

// This structure specifies an evaluation sample.
struct Sample {
  Sentence hypothesis;
  std::vector<Sentence> references;
};

// Evaluator interface.
// The Evaluator performs 3 separated steps:
// "prepare", "map", and "integrate".
// Usage:
//   AnEvaluator eval(params);
//   for (int i  0; i < N; ++i) {
//     Sample sample { hyp[i], { ref1[i], ref2[i], ... } };
//     eval.prepare(sample);
//   }
//   Statistics accum;
//   for (auto& sample : gen) {
//     Sample sample { hyp[i], { ref1[i], ref2[i], ... } };
//     accum += eval.map(sample);
//   }
//   double score = eval.integrate(accum);
class Evaluator {
  Evaluator(const Evaluator&) = delete;
  Evaluator(Evaluator&&) = delete;
  Evaluator& operator =(const Evaluator&) = delete;
  Evaluator& operator =(Evaluator&&) = delete;

public:
  // Derived Evaluators should clarify a list of acceptable EvaluatorParams
  // using constructor's comment.
  Evaluator(const std::vector<EvaluatorParam>& params) {}
  virtual ~Evaluator() {}

  // Prepares inner statistics used in map() and integrate().
  virtual void prepare(const Sample& sample) = 0;

  // Calculates statistics from an evaluation sample.
  // Derived Evaluators should clarify a list of statistics to be obtained by
  // this method using the comment.
  // Recommended format of statistics names:
  //   "stat-name:param1:param2:..."
  // (e.g. BLEUEvaluator)
  //   "samples" (no param)
  //   "len:hyp" (with 1 param)
  //   "ngram:4:match" (with 2 params)
  virtual Statistics map(const Sample& sample) const = 0;

  // Calculates final evaluation score from accumulated statistics.
  virtual double integrate(const Statistics& stats) const = 0;

  // Retrieves Evaluator's name.
  virtual std::string getName() const = 0;

};

} // namespace MTEVal

