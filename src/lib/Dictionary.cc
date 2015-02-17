#include <mteval/Dictionary.h>

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <vector>

using namespace std;

namespace MTEval {

WordID Dictionary::operator[](const string & word) {
    auto it = ids_.find(word);
    if (it != ids_.end()) return it->second;
    WordID id = ids_.size();
    ids_.insert(std::make_pair(word, id));
    return id;
}

Sentence Dictionary::getSentence(const string & line) {
    vector<string> word_list;
    string trimmed = boost::trim_copy(line);
    if (trimmed.empty()) return Sentence();
    boost::split(word_list, trimmed, boost::is_space(), boost::algorithm::token_compress_on);
    Sentence sent(word_list.size());
    transform(word_list.begin(), word_list.end(), sent.begin(), [&](const string & x) { return (*this)[x]; });
    return sent;

}

} // namespace MTEval

