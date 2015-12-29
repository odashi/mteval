#include <mteval/NISTEvaluator.h>

#include <mteval/utils.h>

#include <cmath>
#include <utility>

using namespace std;

namespace MTEval {

NISTEvaluator::NISTEvaluator(const vector<EvaluatorParam> & params)
  : Evaluator(params)
  , ngram_(5) {

  for (auto & p : params) {
    if (p.name == "ngram") ngram_ = p.int_val;
  }
}

NISTEvaluator::~NISTEvaluator() {}

void NISTEvaluator::prepare(const Sample& sample) {
  int len_ref = sample.references[0].size();
  for (int n = 1; n <= ngram_; ++n) {
    for (int k = 0; k + n <= len_ref; ++k) {
      ++freq_[Utility::makeNGram(sample.references[0], k, n)];
    }
  }

  freq_[Sentence()] += len_ref; // for context of unigram
}

Statistics NISTEvaluator::map(const Sample& sample) const {
  if (!freq_.at(Sentence())) {
    throw runtime_error("evaluator is not prepared");
  }

  Statistics stats;
  stats.addInt("samples", 1);

  int len_hyp = sample.hypothesis.size();
  int len_ref = sample.references[0].size();
  stats.addInt("len:hyp", len_hyp);
  stats.addInt("len:ref", len_ref);

  std::map<Sentence, int> possible;
  int max_n = len_hyp < ngram_ ? len_hyp : ngram_;

  // calculate averaged information weights
  for (int n = 1; n <= max_n; ++n) {
    double matched = 0.0;

    for (int k = 0; k + n <= len_ref; ++k) {
      ++possible[Utility::makeNGram(sample.references[0], k, n)];
    }

    for (int k = 0; k + n <= len_hyp; ++k) {
      Sentence ngram = Utility::makeNGram(sample.hypothesis, k, n);
      Sentence context = Utility::makeNGram(sample.hypothesis, k, n - 1);
      auto it = possible.find(ngram);
      if (it != possible.end() && it->second > 0) {
        --it->second;
        matched +=
          log(static_cast<double>(freq_.at(context)) / freq_.at(ngram));
      }
    }

    stats.addInt("ngram:" + std::to_string(n) + ":hyp", len_hyp - n + 1);
    stats.addReal("ngram:" + std::to_string(n) + ":match", matched);
  }

  return std::move(stats);
}

double NISTEvaluator::integrate(const Statistics& stats) const {
  // beta = -ln2 / (ln3 - ln2)^2
  // NOTE: bp = 0.5 when |hyp|/E[|ref|] == 2/3
  const double log_2 = log(2.0);
  const double base = log(3.0) - log_2;
  const double beta = -log_2 / (base * base);

  // calculate n-gram score
  double np = 0.0;
  for (int n = 0; n < ngram_; ++n) {
    double dd = static_cast<double>(
        stats.getInt("ngram:" + std::to_string(n) + ":hyp"));
    double nn = stats.getReal("ngram:" + std::to_string(n) + ":match");
    if (dd > 0.0) {
      np += nn / (dd * log_2);
    }
  }

  // calculate brevity penalty
  int len_hyp = stats.getInt("len:hyp");
  int len_ref = stats.getInt("len:ref");
  double bp = 1.0;
  if (len_ref > 0) {
    bp = static_cast<double>(len_hyp) / len_ref;
    bp = log(bp < 1.0 ? bp : 1.0);
    bp = exp(beta * bp * bp);
  }

  // calculate final score
  return np * bp;
}

string NISTEvaluator::getName() const {
  return "NIST";
}

} // namespace MTEval

