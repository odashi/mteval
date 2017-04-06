#pragma once

#include <mteval/utils.h>

#include <map>
#include <string>

namespace MTEval {

class Dictionary {

    Dictionary(const Dictionary &) = delete;
    Dictionary & operator=(const Dictionary &) = delete;

public:
    Dictionary() {}
    ~Dictionary() {}

    WordID operator[](const std::string & word);
    Sentence getSentence(const std::string & line);

private:
    std::map<std::string, WordID> ids_;

}; // class Dictionary

} // namespace MTEval

