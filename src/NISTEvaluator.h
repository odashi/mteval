#ifndef MTEVAL_NIST_EVALUATOR_H_
#define MTEVAL_NIST_EVALUATOR_H_

#include "Evaluator.h"

namespace MTEval {

class NISTEvaluator : public Evaluator {

    NISTEvaluator(const NISTEvaluator &) = delete;
    NISTEvaluator & operator=(const NISTEvaluator &) = delete;

public:
    NISTEvaluator();
    ~NISTEvaluator();

    void reset();

    void addSentence(const Sentence & reference, const Sentence & hypothesis);

    double getScore() const;

    std::string getName() const;

private:
    int numerators_[4];
    int denominators_[4];
    int total_ref_;
    int total_hyp_;

}; // class NISTEvaluator

} // namespace MTEval

#endif // MTEVAL_NIST_EVALUATOR_H_

