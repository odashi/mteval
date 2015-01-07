#ifndef MTEVAL_DICTIONARY_H_
#define MTEVAL_DICTIONARY_H_

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

    WordID operator[](const std::string & word) {
        auto it = ids_.find(word);
        if (it != ids_.end()) return it->second;
        WordID id = ids_.size();
        ids_.insert(std::make_pair(word, id));
        return id;
    }

private:
    std::map<std::string, WordID> ids_;

}; // class Dictionary

} // namespace MTEval

#endif // MTEval_DICTIONARY_H_

