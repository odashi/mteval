#ifndef MTEVAL_NIST_EVALUATOR_H_
#define MTEVAL_NIST_EVALUATOR_H_

#include <mteval/Evaluator.h>

#include <map>
#include <vector>

namespace MTEval {

class NISTEvaluator : public Evaluator {

    NISTEvaluator(const NISTEvaluator &) = delete;
    NISTEvaluator & operator=(const NISTEvaluator &) = delete;

public:
    NISTEvaluator(const std::vector<EvaluatorParam> & params);
    ~NISTEvaluator();

    void prepare(const Sentence & reference, const Sentence & hypothesis);

    void calculate(const Sentence & reference, const Sentence & hypothesis);

    double getCumulative() const;

    void resetCumulative();
    
    std::string getName() const;

private:
    std::map<Sentence, int> freq_;

    std::vector<double> numerators_;
    std::vector<int> denominators_;
    int total_len_ref_;
    int total_len_hyp_;

    // hyperparameters
    int ngram_;

}; // class NISTEvaluator

} // namespace MTEval

#endif // MTEVAL_NIST_EVALUATOR_H_

