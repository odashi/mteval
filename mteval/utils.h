#pragma once

#include <fstream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace MTEval {

typedef unsigned int WordID;
typedef std::vector<WordID> Sentence;
typedef std::vector<Sentence> Corpus;

class Utility {

    Utility() = delete;
    Utility(const Utility &) = delete;
    Utility & operator=(const Utility &) = delete;

public:
    // calculation utils
    static Sentence makeNGram(const Sentence & sent, size_t begin, size_t n);
    static int countOverlapping(const Sentence & pattern, const Sentence & sent);
    static int findSubsentence(const Sentence & pattern, const Sentence & sent);
    static double calculateLevenshteinDistance(
      const Sentence & ref, const Sentence & hyp,
      const double sub_weight, const double ins_weight, const double del_weight
    );

    // IO utils
    static std::unique_ptr<std::ifstream> openInputStream(const std::string & filename);

}; // class Utility

} // namespace MTEval

