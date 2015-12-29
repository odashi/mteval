#pragma once

#include <mteval/Evaluator.h>

#include <memory>
#include <string>

namespace MTEval {

class EvaluatorFactory {
    EvaluatorFactory() = delete;
    EvaluatorFactory(const EvaluatorFactory&) = delete;
    EvaluatorFactory(EvaluatorFactory&&) = delete;
    EvaluatorFactory& operator=(const EvaluatorFactory&) = delete;
    EvaluatorFactory& operator=(EvaluatorFactory&&) = delete;

public:
    // Create an Evaluator by the specification string.
    // The format of specification string:
    //   EVALUATOR-NAME:param1=value1:param2:value2:...
    // (e.g. BLEUEvaluator)
    //   BLEU:ngram=4:smooth=0
    static std::shared_ptr<Evaluator> create(const std::string & spec);

};

} // namespace MTEval

