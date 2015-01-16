#ifndef MTEVAL_EVALUATOR_FACTORY_H_
#define MTEVAL_EVALUATOR_FACTORY_H_

#include <mteval/Evaluator.h>

#include <memory>
#include <string>

namespace MTEval {

class EvaluatorFactory {

    EvaluatorFactory() = delete;
    EvaluatorFactory(const EvaluatorFactory &) = delete;
    EvaluatorFactory & operator=(const EvaluatorFactory &) = delete;

public:
    // create evaluator by specified name
    static std::shared_ptr<Evaluator> create(const std::string & name);

}; // class EvaluatorFactory

} // namespace MTEval

#endif // MTEVAL_EVALUATOR_FACTORY_H_
