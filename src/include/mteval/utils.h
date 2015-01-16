#ifndef MTEVAL_UTILS_H_
#define MTEVAL_UTILS_H_

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
    static Sentence makeNGram(const Sentence & sent, size_t begin, size_t n) {
        if (begin + n > sent.size()) throw std::runtime_error("out of range");
        Sentence ret(n);
        for (size_t i = 0; i < n; ++i) ret[i] = sent[i + begin];
        return ret;
    }

    static int countOverlapping(const Sentence & pattern, const Sentence & sent) {
        int n = 0;
        int len_pattern = pattern.size();
        int max_k = sent.size() - len_pattern + 1;
        for (int k = 0; k < max_k; ++k) {
            int m = 1;
            for (int i = 0; i < len_pattern; ++i) {
                if (sent[k + i] != pattern[i]) {
                    m = 0;
                    break;
                }
            }
            n += m;
        }
        return n;
    }

    static int search(const Sentence & pattern, const Sentence & sent) {
        int len_pattern = pattern.size();
        int max_k = sent.size() - len_pattern + 1;
        for (int k = 0; k < max_k; ++k) {
            bool p = true;
            for (int i = 0; i < len_pattern; ++i) {
                if (sent[k + i] != pattern[i]) {
                    p = false;
                    break;
                }
            }
            if (p) return k;
        }
        return -1;
    }

    static std::unique_ptr<std::ifstream> openInputStream(const std::string & filename) {
        std::unique_ptr<std::ifstream> ifs(new std::ifstream(filename));
        if (!ifs->is_open()) {
            throw std::runtime_error("could not open \"" + filename + "\"");
        }
        return ifs;
    }

}; // class Utility

} // namespace MTEval

#endif // MTEVAL_UTILS_H_
