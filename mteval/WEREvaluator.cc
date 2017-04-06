#include <mteval/WEREvaluator.h>

#include <mteval/utils.h>

#include <iostream>
#include <utility>

using namespace std;

namespace MTEval {

WEREvaluator::WEREvaluator(const vector<EvaluatorParam> & params)
  : Evaluator(params)
  , w_sub_(1.0)
  , w_ins_(1.0)
  , w_del_(1.0) {

  for (auto& p : params) {
    if (p.name == "substitute") w_sub_ = p.real_val;
    if (p.name == "insert") w_ins_ = p.real_val;
    if (p.name == "delete") w_del_ = p.real_val;
  }
}

WEREvaluator::~WEREvaluator() {}

void WEREvaluator::prepare(const Sample& sample) {}

Statistics WEREvaluator::map(const Sample& sample) const {
  int len_hyp = sample.hypothesis.size();
  int len_ref = sample.references[0].size();
  if (len_ref == 0 || len_hyp == 0) return Statistics();

  double distance = Utility::calculateLevenshteinDistance(
    sample.references[0], sample.hypothesis,
    w_sub_, w_ins_, w_del_
  );
  
  Statistics stats;
  stats.addInt("samples", 1);
  stats.addInt("distance", distance);
  stats.addReal("score", distance / len_ref);
  return std::move(stats);
}

double WEREvaluator::integrate(const Statistics& stats) const {
  int samples = stats.getInt("samples");
  if (samples == 0) return 0.0;
  else return stats.getReal("score") / samples;
}

string WEREvaluator::getName() const {
  return "WER";
}

} // namespace MTEval

