#include <mteval/EvaluatorFactory.h>

#include <mteval/BLEUEvaluator.h>
#include <mteval/NISTEvaluator.h>
#include <mteval/RIBESEvaluator.h>
#include <mteval/WEREvaluator.h>

#include <stdexcept>

using namespace std;

namespace MTEval {

shared_ptr<Evaluator> EvaluatorFactory::create(const string & name) {
    if (name == "BLEU") return shared_ptr<Evaluator>(new BLEUEvaluator());
    if (name == "NIST") return shared_ptr<Evaluator>(new NISTEvaluator());
    if (name == "RIBES") return shared_ptr<Evaluator>(new RIBESEvaluator());
    if (name == "WER") return shared_ptr<Evaluator>(new WEREvaluator());
    throw runtime_error("unknown evaluator name \"" + name + "\"");
}

} // namespace MTEval

