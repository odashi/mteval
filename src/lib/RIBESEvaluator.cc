#include <mteval/RIBESEvaluator.h>

#include <mteval/utils.h>

#include <cmath>
#include <stdexcept>
#include <utility>

using namespace std;

namespace MTEval {

RIBESEvaluator::RIBESEvaluator(const vector<EvaluatorParam> & params)
  : Evaluator(params)
  , alpha_(0.25)
  , beta_(0.1) {

  for (auto & p : params) {
    if (p.name == "alpha") alpha_ = p.real_val;
    else if (p.name == "beta") beta_ = p.real_val;
  }
}

RIBESEvaluator::~RIBESEvaluator() {}

void RIBESEvaluator::prepare(const Sample& sample) {}

Statistics RIBESEvaluator::map(const Sample& sample) const {
  int len_hyp = sample.hypothesis.size();
  int len_ref = sample.references[0].size();

  if (len_ref == 0) throw runtime_error("reference has no any words");
  
  // statistics
  double nkt = 0.0; // kendall"s tau
  double prec = 0.0; // unigram precision
  double bp = 0.0; // brevity penalty

  if (len_hyp > 0) {
    // calculate brevity penalty
    bp = exp(1.0 - static_cast<double>(len_ref) / len_hyp);
    if (bp > 1.0) bp = 1.0;

    // calculate alignment
    vector<int> aligned;
    
    for (int i = 0; i < len_hyp; ++i) {
      WordID hw = sample.hypothesis[i];
      int in_ref = 0;
      int align = -1;
      for (int k = 0; k < len_ref; ++k) {
        if (sample.references[0][k] == hw) {
          ++in_ref;
          align = k;
        }
      }
      if (in_ref == 0) continue;
      
      int in_hyp = 0;
      for (int k = 0; k < len_hyp; ++k) {
        if (sample.hypothesis[k] == hw) {
          ++in_hyp;
        }
      }

      if (in_ref == 1 && in_hyp == 1) {
        aligned.push_back(align);
      } else {
        int j = len_hyp - i;
        int max_window = i > j ? i : j;
        for (int window = 1; window <= max_window; ++window) {
          if (window <= i) {
            Sentence ngram =
              Utility::makeNGram(sample.hypothesis, i - window, window + 1);
            if (Utility::countOverlapping(ngram, sample.references[0]) == 1 &&
              Utility::countOverlapping(ngram, sample.hypothesis) == 1) {
              aligned.push_back(
                  Utility::findSubsentence(ngram, sample.references[0]) +
                  window);
              break;
            }
          }
          if (i + window < len_hyp) {
            Sentence ngram = Utility::makeNGram(
                sample.hypothesis, i, window + 1);
            if (Utility::countOverlapping(ngram, sample.references[0]) == 1 &&
              Utility::countOverlapping(ngram, sample.hypothesis) == 1) {
              aligned.push_back(
                  Utility::findSubsentence(ngram, sample.references[0]));
              break;
            }
          }
        }
      }
    }

    // calculate kendall's tau and unigram precision
    int len_aligned = aligned.size();
    if (len_aligned == 1 && len_ref == 1) {
      nkt = 1.0;
      prec = 1.0 / len_hyp;
    } else if (len_aligned < 2) {
      //nkt = 0.0;
      //prec = 0.0;
    } else {
      int ascending = 0;
      for (int i = 0; i < len_aligned - 1; ++i) {
        for (int j = i + 1; j < len_aligned; ++j) {
          if (aligned[i] < aligned[j]) ++ascending;
        }
      }
      nkt = static_cast<double>(ascending) /
        ((len_aligned * (len_aligned - 1)) >> 1);
      prec = 1.0 * len_aligned / len_hyp;
    }
  }

  // calculate final score
  double score = nkt * pow(prec, alpha_) * pow(bp, beta_);

  Statistics stats;
  stats.addReal("brevity", bp);
  stats.addReal("nkt", nkt);
  stats.addReal("prec", prec);
  stats.addInt("samples", 1);
  stats.addReal("score", score);
  return std::move(stats);
}

double RIBESEvaluator::integrate(const Statistics& stats) const {
  int samples = stats.getInt("samples");
  if (samples == 0) return 0.0;
  else return stats.getReal("score") / samples;
}

string RIBESEvaluator::getName() const {
  return "RIBES";
}

} // namespace MTEval

