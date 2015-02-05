#ifndef MTEVAL_EVALUATOR_H_
#define MTEVAL_EVALUATOR_H_

#include <mteval/utils.h>

#include <string>
#include <vector>

namespace MTEval {

struct EvaluatorParam {
    std::string name;
    int int_val;
    double real_val;
}; // struct EvaluatorParam

class Evaluator {

    Evaluator(const Evaluator &) = delete;
    Evaluator & operator =(const Evaluator &) = delete;

public:
    Evaluator(const std::vector<EvaluatorParam> & params) {}
    virtual ~Evaluator() {}

    virtual void prepare(const Sentence & reference, const Sentence & hypothesis) = 0;

    virtual void calculate(const Sentence & reference, const Sentence & hypothesis) = 0;

    virtual double getCumulative() const = 0;

    virtual void resetCumulative() = 0;
    
    virtual std::string getName() const = 0;

}; // class Evaluator

} // namespace MTEVal

#endif // MTEVAL_EVALUATOR_H_

