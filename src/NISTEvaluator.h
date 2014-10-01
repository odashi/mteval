#ifndef MTEVAL_NIST_EVALUATOR_H_
#define MTEVAL_NIST_EVALUATOR_H_

#include "Evaluator.h"

#include <map>

namespace MTEval {

class NISTEvaluator : public Evaluator {

    NISTEvaluator(const NISTEvaluator &) = delete;
    NISTEvaluator & operator=(const NISTEvaluator &) = delete;

public:
    NISTEvaluator();
    ~NISTEvaluator();

    void prepare(const Sentence & reference, const Sentence & hypothesis);

    void calculate(const Sentence & reference, const Sentence & hypothesis);

    double getCumulative() const;

    void resetCumulative();
    
    std::string getName() const;

private:
    std::map<Sentence, int> freq_;
    int total_len_ref_;
    int num_sents_;

    int num_cumulative_;
    double cumulative_;

    double beta_; // factor of brevity penalty

}; // class NISTEvaluator

} // namespace MTEval

#endif // MTEVAL_NIST_EVALUATOR_H_

