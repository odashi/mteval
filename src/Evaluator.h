#ifndef MTEVAL_EVALUATOR_H_
#define MTEVAL_EVALUATOR_H_

#include "utils.h"

#include <string>

namespace MTEval {

class Evaluator {

    Evaluator(const Evaluator &) = delete;
    Evaluator & operator =(const Evaluator &) = delete;

public:
    Evaluator() {}
    virtual ~Evaluator() {}

    virtual void reset() = 0;

    virtual void addSentence(const Sentence & reference, const Sentence & hypothesis) = 0;

    virtual double getScore() const = 0;

    virtual std::string getName() const = 0;

}; // class Evaluator

} // namespace MTEVal

#endif // MTEVAL_EVALUATOR_H_

