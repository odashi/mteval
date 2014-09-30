#ifndef MTEVAL_BLEU_EVALUATOR_H_
#define MTEVAL_BLEU_EVALUATOR_H_

#include "Evaluator.h"

namespace MTEval {

class BLEUEvaluator : public Evaluator {

    BLEUEvaluator(const BLEUEvaluator &) = delete;
    BLEUEvaluator & operator=(const BLEUEvaluator &) = delete;

public:
    BLEUEvaluator();
    ~BLEUEvaluator();

    void reset();

    void addSentence(const Sentence & reference, const Sentence & hypothesis);

    double getScore() const;

    std::string getName() const;

private:
    int numerators_[4];
    int denominators_[4];
    int total_ref_;
    int total_hyp_;

}; // class BLEUEvaluator

} // namespace MTEval

#endif // MTEVAL_BLEU_EVALUATOR_H_

