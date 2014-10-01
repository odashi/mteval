#include "NISTEvaluator.h"

#include "utils.h"

#include <cmath>

using namespace std;

namespace MTEval {

NISTEvaluator::NISTEvaluator() {
    total_len_ref_ = 0;
    num_sents_ = 0;
    
    // beta = -ln2 / (ln3 - ln2)^2
    // note: bp = 0.5 when |hyp|/E[|ref|] == 2/3
    beta_ = log(3.0) - log(2.0);
    beta_ = -log(2.0) / (beta_ * beta_);

    resetCumulative();
}

NISTEvaluator::~NISTEvaluator() {}

void NISTEvaluator::prepare(const Sentence & reference, const Sentence & hypothesis) {
    int len_ref = reference.size();
    for (int n = 0; n < 5; ++n) {
        for (int k = 0; k + n < len_ref; ++k) {
            ++freq_[Utility::makeNGram(reference, k, n + 1)];
        }
    }

    total_len_ref_ += len_ref;
    freq_[Sentence()] += len_ref; // for context of unigram
    ++num_sents_;
}

void NISTEvaluator::calculate(const Sentence & reference, const Sentence & hypothesis) {
    if (!num_sents_) throw runtime_error("evaluator is not prepared");

    int len_ref = reference.size();
    int len_hyp = hypothesis.size();
    
    // calculate brevity penalty

    double bp = len_hyp * num_sents_ / static_cast<double>(total_len_ref_);
    bp = log(bp < 1.0 ? bp : 1.0);
    bp = exp(beta_ * bp * bp);

    // calculate averaged information weights

    map<Sentence, int> possible;
    int max_n = len_hyp < 5 ? len_hyp : 5;
    double score = 0.0;

    for (int n = 0; n < max_n; ++n) {
        double sum = 0;

        for (int k = 0; k + n < len_ref; ++k) {
            ++possible[Utility::makeNGram(reference, k, n + 1)];
        }

        for (int k = 0; k + n < len_hyp; ++k) {
            Sentence ngram = Utility::makeNGram(hypothesis, k, n + 1);
            Sentence context = Utility::makeNGram(hypothesis, k, n);
            auto it = possible.find(ngram);
            if (it != possible.end() && it->second > 0) {
                --it->second;
                sum += log(static_cast<double>(freq_[context]) / freq_[ngram]);
            }
        }

        score += sum / (len_hyp - n);
    }

    cumulative_ += score * bp;
    ++num_cumulative_;
}

double NISTEvaluator::getCumulative() const {
    if (num_cumulative_) return cumulative_ / (num_cumulative_ * log(2.0));
    else return 0.0;
}

void NISTEvaluator::resetCumulative() {
    num_cumulative_ = 0;
    cumulative_ = 0.0;
}

string NISTEvaluator::getName() const {
    return "NIST";
}

} // namespace MTEval

