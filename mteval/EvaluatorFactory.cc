#include <mteval/EvaluatorFactory.h>

#include <mteval/BLEUEvaluator.h>
#include <mteval/NISTEvaluator.h>
#include <mteval/RIBESEvaluator.h>
#include <mteval/WEREvaluator.h>

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <cctype>
#include <stdexcept>

using namespace std;

namespace MTEval {

shared_ptr<Evaluator> EvaluatorFactory::create(const string & spec) {
  vector<string> settings;
  boost::split(settings, spec, boost::is_any_of(":"));

  vector<EvaluatorParam> params;
  for (size_t i = 1; i < settings.size(); ++i) {
    vector<string> keyval;
    boost::split(keyval, settings[i], boost::is_any_of("="));
    if (keyval.size() != 2) {
      throw runtime_error("invalid evaluator specification \"" + spec + "\"");
    }

    EvaluatorParam p;
    p.name = keyval[0];
    try { p.int_val = stoi(keyval[1]); } catch (...) { p.int_val = 0; }
    try { p.real_val = stod(keyval[1]); } catch (...) { p.real_val = 0; }
    params.push_back(p);
  }

  string name = settings[0];
  transform(name.begin(), name.end(), name.begin(), ::tolower);

  if (name == "bleu")
    return shared_ptr<Evaluator>(new BLEUEvaluator(params));
  if (name == "nist")
    return shared_ptr<Evaluator>(new NISTEvaluator(params));
  if (name == "ribes")
    return shared_ptr<Evaluator>(new RIBESEvaluator(params));
  if (name == "wer")
    return shared_ptr<Evaluator>(new WEREvaluator(params));
  throw runtime_error("invalid evaluator specification \"" + spec + "\"");
}

} // descspace MTEval

