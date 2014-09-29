#ifndef MTEVAL_EVALUATOR_H_
#define MTEVAL_EVALUATOR_H_

#include <vector>

namespace MTEval {

class Evaluator {

    Evaluator(const Evaluator &) = delete;
    Evaluator & operator =(const Evaluator &) = delete;

public:
    Evaluator() {}
    virtual ~Evaluator() {}

    void addSentence(
        const std::vector<unsigned int> & reference,
        const std::vector<unsigned int> & hypothesis) = 0;

    double getScore() const = 0;

}; // class Evaluator

} // namespace MTEVal

#endif // MTEVAL_EVALUATOR_H_

