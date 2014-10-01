#ifndef MTEVAL_RIBES_EVALUATOR_H_
#define MTEVAL_RIBES_EVALUATOR_H_

#include "Evaluator.h"

namespace MTEval {

class RIBESEvaluator : public Evaluator {

    RIBESEvaluator(const RIBESEvaluator &) = delete;
    RIBESEvaluator & operator=(const RIBESEvaluator &) = delete;

public:
    RIBESEvaluator();
    ~RIBESEvaluator();

    void prepare(const Sentence & reference, const Sentence & hypothesis);

    void calculate(const Sentence & reference, const Sentence & hypothesis);

    double getCumulative() const;

    void resetCumulative();
    
    std::string getName() const;

private:
    unsigned int num_sents_;
    double total_;

}; // class RIBESEvaluator

} // namespace MTEval

#endif // MTEVAL_RIBES_EVALUATOR_H_

