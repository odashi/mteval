#ifndef MTEVAL_WER_EVALUATOR_H_
#define MTEVAL_WER_EVALUATOR_H_

#include <mteval/Evaluator.h>

namespace MTEval {

class WEREvaluator : public Evaluator {

    WEREvaluator(const WEREvaluator &) = delete;
    WEREvaluator & operator=(const WEREvaluator &) = delete;

public:
    WEREvaluator(const std::vector<EvaluatorParam> & params);
    ~WEREvaluator();

    void prepare(const Sentence & reference, const Sentence & hypothesis);

    void calculate(const Sentence & reference, const Sentence & hypothesis);

    double getCumulative() const;

    void resetCumulative();
    
    std::string getName() const;

private:
    unsigned int num_sents_;
    double total_;

}; // class WEREvaluator

} // namespace MTEval

#endif // MTEVAL_WER_EVALUATOR_H_

