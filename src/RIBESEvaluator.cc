#include "RIBESEvaluator.h"

#include "utils.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include <cstdio>

using namespace std;

namespace MTEval {

RIBESEvaluator::RIBESEvaluator() {
    reset();
}

RIBESEvaluator::~RIBESEvaluator() {}

void RIBESEvaluator::reset() {
    num_sents_ = 0;
    total_ = 0.0;
}

void RIBESEvaluator::addSentence(const Sentence & reference, const Sentence & hypothesis) {
    int len_ref = reference.size();
    int len_hyp = hypothesis.size();

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
            WordID hw = hypothesis[i];
            int in_ref = 0;
            int align = -1;
            for (int k = 0; k < len_ref; ++k) {
                if (reference[k] == hw) {
                    ++in_ref;
                    align = k;
                }
            }
            if (in_ref == 0) continue;
            
            int in_hyp = 0;
            for (int k = 0; k < len_hyp; ++k) {
                if (hypothesis[k] == hw) {
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
                        Sentence ngram = Utility::makeNGram(hypothesis, i - window, window + 1);
                        if (Utility::countOverlapping(ngram, reference) == 1 &&
                            Utility::countOverlapping(ngram, hypothesis) == 1) {
                            aligned.push_back(Utility::search(ngram, reference) + window);
                            break;
                        }
                    }
                    if (i + window < len_hyp) {
                        Sentence ngram = Utility::makeNGram(hypothesis, i, window + 1);
                        if (Utility::countOverlapping(ngram, reference) == 1 &&
                            Utility::countOverlapping(ngram, hypothesis) == 1) {
                            aligned.push_back(Utility::search(ngram, reference));
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
            nkt = static_cast<double>(ascending) / ((len_aligned * (len_aligned - 1)) >> 1);
            prec = 1.0 * len_aligned / len_hyp;
        }
    }

    // calculate final score
    double score = nkt * pow(prec, 0.25) * pow(bp, 0.1);

    ++num_sents_;
    total_ += score;
}

double RIBESEvaluator::getScore() const {
    if (num_sents_) return total_ / static_cast<double>(num_sents_);
    else return 0.0;
}

string RIBESEvaluator::getName() const {
    return "RIBES";
}

} // namespace MTEval

