#pragma once

#include <mteval/Evaluator.h>

#include <vector>

namespace MTEval {

class BLEUEvaluator : public Evaluator {

    BLEUEvaluator(const BLEUEvaluator &) = delete;
    BLEUEvaluator & operator=(const BLEUEvaluator &) = delete;

public:
    BLEUEvaluator(const std::vector<EvaluatorParam> & params);
    ~BLEUEvaluator();

    void prepare(const Sentence & reference, const Sentence & hypothesis);

    void calculate(const Sentence & reference, const Sentence & hypothesis);

    double getCumulative() const;

    void resetCumulative();
    
    std::string getName() const;

private:
    std::vector<int> numerators_;
    std::vector<int> denominators_;
    int total_ref_;
    int total_hyp_;

    // hyperparameters
    int ngram_;
    double smooth_;

}; // class BLEUEvaluator

} // namespace MTEval

