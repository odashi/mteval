#include "BLEUEvaluator.h"

#include "utils.h"

#include <cmath>
#include <map>

using namespace std;

namespace MTEval {

BLEUEvaluator::BLEUEvaluator() {
    resetCumulative();
}

BLEUEvaluator::~BLEUEvaluator() {}

void BLEUEvaluator::prepare(const Sentence & reference, const Sentence & hypothesis) {
}

void BLEUEvaluator::calculate(const Sentence & reference, const Sentence & hypothesis) {
    int len_ref = reference.size();
    int len_hyp = hypothesis.size();
    total_ref_ += len_ref;
    total_hyp_ += len_hyp;

    map<Sentence, int> possible;
    int max_n = len_hyp < 4 ? len_hyp : 4;

    // gather statistics
    for (int n = 0; n < max_n; ++n) {
        denominators_[n] += len_hyp - n;

        for (int k = 0; k + n < len_ref; ++k) {
            ++possible[Utility::makeNGram(reference, k, n + 1)];
        }

        for (int k = 0; k + n < len_hyp; ++k) {
            auto it = possible.find(Utility::makeNGram(hypothesis, k, n + 1));
            if (it != possible.end() && it->second > 0) {
                --it->second;
                ++numerators_[n];
            }
        }
    }
}

double BLEUEvaluator::getCumulative() const {
    // calculate precision
    double np = 0.0;
    for (int n = 0; n < 4; ++n) {
        if (numerators_[n] == 0) return 0.0;
        np += log(static_cast<double>(numerators_[n])) - log(static_cast<double>(denominators_[n]));
    }

    // calculate brevity penalty
    double bp = 1.0 - static_cast<double>(total_ref_) / total_hyp_;
    if (bp > 0.0) bp = 0.0;

    // calculate final score
    return exp(np / 4.0 + bp);
}

void BLEUEvaluator::resetCumulative() {
    for (int n = 0; n < 4; ++n) {
        numerators_[n] = 0;
        denominators_[n] = 0;
    }
    total_ref_ = 0;
    total_hyp_ = 0;
}

string BLEUEvaluator::getName() const {
    return "BLEU";
}

} // namespace MTEval

