#include <mteval/WEREvaluator.h>

#include <iostream>
#include <utility>

using namespace std;

namespace MTEval {

WEREvaluator::WEREvaluator(const vector<EvaluatorParam> & params)
  : Evaluator(params) {}

WEREvaluator::~WEREvaluator() {}

void WEREvaluator::prepare(const Sample& sample) {}

Statistics WEREvaluator::map(const Sample& sample) const {
  int len_hyp = sample.hypothesis.size();
  int len_ref = sample.references[0].size();
  if (len_ref == 0 || len_hyp == 0) return Statistics();

  // initialize

  int dp[len_ref + 1][len_hyp + 1];
  for (int i = 0; i <= len_ref; ++i) dp[i][0] = i;
  for (int j = 1; j <= len_hyp; ++j) dp[0][j] = j;

  // calculate Levenshtein distance

  for (int i = 1; i <= len_ref; ++i) {
    for (int j = 1; j <= len_hyp; ++j) {
      if (sample.references[0][i - 1] == sample.hypothesis[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      }
      else {
        int sub = dp[i - 1][j - 1];
        int ins = dp[i][j - 1];
        int del = dp[i - 1][j];
        int cur = sub < ins ? sub : ins;
        cur = cur < del ? cur : del;
        dp[i][j] = 1 + cur;
      }
    }
  }

  Statistics stats;
  stats.addInt("samples", 1);
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

