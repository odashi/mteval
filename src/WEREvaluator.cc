#include "WEREvaluator.h"

using namespace std;

namespace MTEval {

WEREvaluator::WEREvaluator() {
    reset();
}

WEREvaluator::~WEREvaluator() {}

void WEREvaluator::reset() {
    num_sents_ = 0;
    total_ = 0.0;
}

void WEREvaluator::addSentence(const Sentence & reference, const Sentence & hypothesis) {
    ++num_sents_;

    int len_ref = reference.size();
    int len_hyp = hypothesis.size();
    if (len_ref == 0 || len_hyp == 0) return;

    // initialize

    int dp[len_ref + 1][len_hyp + 1];
    for (int i = 0; i <= len_ref; ++i) dp[i][0] = i;
    for (int j = 1; j <= len_hyp; ++j) dp[0][j] = j;

    // calculate Levenshtein distance

    for (int i = 1; i <= len_ref; ++i) {
        for (int j = 1; j <= len_hyp; ++j) {
            if (reference[i - 1] == hypothesis[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else {
                int sub = dp[i - 1][j - 1];
                int ins = dp[i][j - 1];
                int del = dp[i - 1][j];
                dp[i][j] = 1 + (sub < ins ? sub : ins < del ? ins : del);
            }
        }
    }

    total_ += static_cast<double>(dp[len_ref][len_hyp]) / len_ref;
}

double WEREvaluator::getScore() const {
    if (num_sents_) return total_ / static_cast<double>(num_sents_);
    else return 0.0;
}

string WEREvaluator::getName() const {
    return "WER";
}

} // namespace MTEval

