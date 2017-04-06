#include <mteval/BLEUEvaluator.h>

#include <mteval/utils.h>

#include <cmath>
#include <map>
#include <utility>

namespace MTEval {

BLEUEvaluator::BLEUEvaluator(const std::vector<EvaluatorParam>& params)
  : Evaluator(params)
  , ngram_(4)
  , smooth_(0.0) {

  for (auto& p : params) {
    if (p.name == "ngram") ngram_ = p.int_val;
    if (p.name == "smooth") smooth_ = p.real_val;
  }
}

BLEUEvaluator::~BLEUEvaluator() {}

void BLEUEvaluator::prepare(const Sample& sample) {}

Statistics BLEUEvaluator::map(const Sample& sample) const {
  Statistics stats;
  stats.addInt("samples", 1);

  int len_hyp = sample.hypothesis.size();
  int len_ref = sample.references[0].size();
  stats.addInt("len:hyp", len_hyp);
  stats.addInt("len:ref", len_ref);

  std::map<Sentence, int> possible;
  int max_n = len_hyp < ngram_ ? len_hyp : ngram_;

  // gather statistics
  for (int n = 1; n <= max_n; ++n) {
    int matched = 0;

    for (int k = 0; k + n <= len_ref; ++k) {
      ++possible[Utility::makeNGram(sample.references[0], k, n)];
    }

    for (int k = 0; k + n <= len_hyp; ++k) {
      auto it = possible.find(Utility::makeNGram(sample.hypothesis, k, n));
      if (it != possible.end() && it->second > 0) {
        --it->second;
        ++matched;
      }
    }
    
    stats.addInt("ngram:" + std::to_string(n) + ":hyp", len_hyp - n + 1);
    stats.addInt("ngram:" + std::to_string(n) + ":match", matched);
  }

  return std::move(stats);
}

double BLEUEvaluator::integrate(const Statistics& stats) const {
  // calculate precision
  double np = 0.0;
  for (int n = 1; n <= ngram_; ++n) {
    double dd = static_cast<double>(
        stats.getInt("ngram:" + std::to_string(n) + ":hyp"));
    double nn = static_cast<double>(
        stats.getInt("ngram:" + std::to_string(n) + ":match"));
    
    // smoothing
    if (n > 1) {
      nn += smooth_;
      dd += smooth_;
    }

    // if at least 1 numerator is 0, the score becomes 0.
    // when 'smooth' is set as nonzero, this line evaluates whether only 1-gram precision is 0 or not.
    if (nn == 0.0) return 0.0;
    
    np += log(nn) - log(dd);
  }

  // calculate brevity penalty
  double bp =
    1.0 -
    static_cast<double>(stats.getInt("len:ref")) /
    static_cast<double>(stats.getInt("len:hyp"));
  if (bp > 0.0) bp = 0.0;

  // calculate final score
  return exp(np / static_cast<double>(ngram_) + bp);
}

std::string BLEUEvaluator::getName() const {
  return "BLEU";
}

} // namespace MTEval

