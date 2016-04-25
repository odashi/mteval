#include <mteval/WEREvaluator.h>

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

  // initialize

  double dp[len_ref + 1][len_hyp + 1];
  for (int i = 0; i <= len_ref; ++i) {
    dp[i][0] = w_del_ * static_cast<double>(i);
  }
  for (int j = 1; j <= len_hyp; ++j) {
    dp[0][j] = w_ins_ * static_cast<double>(j);
  }

  // calculate Levenshtein distance

  for (int i = 1; i <= len_ref; ++i) {
    for (int j = 1; j <= len_hyp; ++j) {
      if (sample.references[0][i - 1] == sample.hypothesis[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      }
      else {
        double sub = w_sub_ + dp[i - 1][j - 1];
        double ins = w_ins_ + dp[i][j - 1];
        double del = w_del_ + dp[i - 1][j];
        double tmp = sub < ins ? sub : ins;
        dp[i][j] = tmp < del ? tmp : del;
      }
    }
  }

  Statistics stats;
  stats.addInt("samples", 1);
  stats.addInt("distance", dp[len_ref][len_hyp]);
  stats.addReal("score", static_cast<double>(dp[len_ref][len_hyp]) / len_ref);
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

