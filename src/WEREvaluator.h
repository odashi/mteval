#ifndef MTEVAL_WER_EVALUATOR_H_
#define MTEVAL_WER_EVALUATOR_H_

#include "Evaluator.h"

namespace MTEval {

class WEREvaluator : public Evaluator {

    WEREvaluator(const WEREvaluator &) = delete;
    WEREvaluator & operator=(const WEREvaluator &) = delete;

public:
    WEREvaluator();
    ~WEREvaluator();

    void reset();

    void addSentence(const Sentence & reference, const Sentence & hypothesis);

    double getScore() const;

    std::string getName() const;

private:
    unsigned int num_sents_;
    double total_;

}; // class WEREvaluator

} // namespace MTEval

#endif // MTEVAL_WER_EVALUATOR_H_

