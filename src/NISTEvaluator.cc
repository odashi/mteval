#include "NISTEvaluator.h"

#include "utils.h"

#include <cmath>
#include <map>

using namespace std;

namespace MTEval {

NISTEvaluator::NISTEvaluator() {
    reset();
}

NISTEvaluator::~NISTEvaluator() {}

void NISTEvaluator::reset() {
}

void NISTEvaluator::addSentence(const Sentence & reference, const Sentence & hypothesis) {
    return;
}

double NISTEvaluator::getScore() const {
    return 0.0;
}

string NISTEvaluator::getName() const {
    return "NIST";
}

} // namespace MTEval

