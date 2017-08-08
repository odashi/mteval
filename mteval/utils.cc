#include <mteval/utils.h>

using namespace std;

namespace MTEval {

Sentence Utility::makeNGram(const Sentence & sent, size_t begin, size_t n) {
    if (begin + n > sent.size()) throw runtime_error("out of range");
    Sentence ret(n);
    for (size_t i = 0; i < n; ++i) ret[i] = sent[i + begin];
    return ret;
}

int Utility::countOverlapping(const Sentence & pattern, const Sentence & sent) {
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

int Utility::findSubsentence(const Sentence & pattern, const Sentence & sent) {
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

double Utility::calculateLevenshteinDistance(
  const Sentence & ref, const Sentence & hyp,
  const double sub_weight, const double ins_weight, const double del_weight
) {
  int len_ref = ref.size();
  int len_hyp = hyp.size();

  // initialize DP table as a matrix with len_ref + 1 rows and len_hyp + 1 columns
  vector< vector<double> > dp(len_ref + 1, vector<double>(len_hyp + 1));

  for (int i = 0; i <= len_ref; ++i) {
    dp[i][0] = del_weight * static_cast<double>(i);
  }
  for (int j = 1; j <= len_hyp; ++j) {
    dp[0][j] = ins_weight * static_cast<double>(j);
  }

  // fill DP table
  for (int i = 1; i <= len_ref; ++i) {
    for (int j = 1; j <= len_hyp; ++j) {
      if (ref[i - 1] == hyp[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        double sub = sub_weight + dp[i - 1][j - 1];
        double ins = ins_weight + dp[i][j - 1];
        double del = del_weight + dp[i - 1][j];
        double tmp = sub < ins ? sub : ins;
        dp[i][j] = tmp < del ? tmp : del;
      }
    }
  }

  return dp[len_ref][len_hyp];
}

unique_ptr<ifstream> Utility::openInputStream(const string & filename) {
    unique_ptr<ifstream> ifs(new ifstream(filename));
    if (!ifs->is_open()) {
        throw runtime_error("could not open \"" + filename + "\"");
    }
    return ifs;
}

} // namespace MTEval

