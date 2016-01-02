#include <mteval/utils.h>
#include <mteval/Dictionary.h>
#include <mteval/Evaluator.h>
#include <mteval/EvaluatorFactory.h>

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <memory>
#include <vector>

using namespace std;
using namespace MTEval;

boost::program_options::variables_map parseOptions(int argc, char * argv []) {
  namespace OPT = boost::program_options;
  string description = "Operate pairwise bootstrap resampling test";
  string binname = "mteval-pairwise";

  // set options

  OPT::options_description opt_generic("Generic options");
  opt_generic.add_options()
    ("help", "show this help and exit")
    ("verbose,v", "show intermediate results")
    ;
  OPT::options_description opt_input("Input files");
  opt_input.add_options()
    ("reference,r", OPT::value<string>(),
     "(required) reference file")
    ("hypothesis,h", OPT::value<vector<string> >()->multitoken(),
     "(required) 2 hypothesis file")
    ;
  OPT::options_description opt_config("Configurations");
  opt_config.add_options()
    ("evaluator,e", OPT::value<vector<string> >()->multitoken(),
     "(required) evaluator specifications")
    ("iteration,i", OPT::value<int>(),
     "(required) number of virtual test sets to evaluate")
    ("sample,s", OPT::value<int>(),
     "(required) number of sentences in a virtual test sets")
    ;
  OPT::options_description opt;
  opt.add(opt_generic).add(opt_input).add(opt_config);

  // parse

  OPT::variables_map args;
  OPT::store(OPT::parse_command_line(argc, argv, opt), args);
  OPT::notify(args);

  // process usage

  if (args.count("help")) {
    cerr << description << endl;
    cerr << "Usage: " << binname
       << " [options] -i INT -s INT -e EVAL1 EVAL2 ..."
       << " -r REF -h HYP1 HYP2" << endl;
    cerr << opt << endl;
    exit(1);
  }

  // check required options

  if (!args.count("reference") ||
    !args.count("hypothesis") ||
    !args.count("evaluator") ||
    !args.count("iteration") ||
    !args.count("sample")) {
    cerr << "ERROR: insufficient required options" << endl;
    cerr << "(--help to show usage)" << endl;
    exit(1);
  }

  return move(args);
}

Corpus loadCorpus(const string & filename, Dictionary & dict) {
  Corpus corpus;
  auto ifs = Utility::openInputStream(filename);
  string line;
  while (getline(*ifs, line)) {
    corpus.push_back(move(dict.getSentence(line)));
  }
  return move(corpus);
}

int main(int argc, char * argv[]) {
  try {
    auto args = parseOptions(argc, argv);

    bool verbose = args.count("verbose");

    vector<string> evaluator_spec = args["evaluator"].as<vector<string> >();
    int num_iteration = args["iteration"].as<int>();
    int num_sample = args["sample"].as<int>();
    string filename_ref = args["reference"].as<string>();
    vector<string> filename_hyp = args["hypothesis"].as<vector<string> >();

    // Argument validation
    if (num_iteration <= 0)
      throw runtime_error("-i must be greater than 0");
    if (num_sample <= 0)
      throw runtime_error("-s must be greater than 0");
    if (filename_hyp.size() != 2)
      throw runtime_error("-h must be set just 2 files");

    // Get evaluators
    int num_evaluators = evaluator_spec.size();
    vector<shared_ptr<Evaluator> > evaluators1, evaluators2;
    for (const string & spec : evaluator_spec) {
      evaluators1.push_back(EvaluatorFactory::create(spec));
      evaluators2.push_back(EvaluatorFactory::create(spec));
    }

    Dictionary dict;

    // Load corpus
    Corpus corpus_hyp1 = loadCorpus(filename_hyp[0], dict);
    Corpus corpus_hyp2 = loadCorpus(filename_hyp[1], dict);
    Corpus corpus_ref = loadCorpus(filename_ref, dict);

    size_t num_sentences = corpus_ref.size();
    if (corpus_hyp1.size() != num_sentences ||
        corpus_hyp2.size() != num_sentences) {
      throw runtime_error("corpus size are different");
    }

    // Number of winner (hyp1 > hyp2)
    vector<int> won(evaluators1.size(), 0);

    // Randomizer
    random_device devrand;
    mt19937 mtrand(devrand());
    uniform_int_distribution<int> dist(0, num_sentences - 1);
    auto next = [&](){ return dist(mtrand); };

    for (int iteration = 0; iteration < num_iteration; ++iteration) {
      // Choose sentences and prepare evaluators
      vector<Sample> samples1;
      vector<Sample> samples2;
      for (int s = 0; s < num_sample; ++s) {
        int id = next();
        samples1.push_back({corpus_hyp1[id], {corpus_ref[id]}});
        samples2.push_back({corpus_hyp2[id], {corpus_ref[id]}});
        for (int ev = 0; ev < num_evaluators; ++ev) {
          evaluators1[ev]->prepare(samples1.back());
          evaluators2[ev]->prepare(samples2.back());
        }
      }

      // Analyze
      vector<Statistics> stats1(num_evaluators);
      vector<Statistics> stats2(num_evaluators);
      for (size_t t = 0; t < samples1.size(); ++t) {
        for (int ev = 0; ev < num_evaluators; ++ev) {
          stats1[ev] += evaluators1[ev]->map(samples1[t]);
          stats2[ev] += evaluators2[ev]->map(samples2[t]);
        }
      }

      // Retrieve scores
      vector<double> scores1(num_evaluators);
      vector<double> scores2(num_evaluators);
      for (int ev = 0; ev < num_evaluators; ++ev) {
        scores1[ev] = evaluators1[ev]->integrate(stats1[ev]);
        scores2[ev] = evaluators2[ev]->integrate(stats2[ev]);
      }

      // Print results
      if (verbose) {
        cout << "Iteration " << (iteration + 1) << ":" << endl;
        for (int ev = 0; ev < num_evaluators; ++ev) {
          if (ev > 0) cout << '\t';
          cout << (boost::format("%s=%.6f") %
              evaluators1[ev]->getName() %
              scores1[ev]);
        }
        cout << endl;
        for (int ev = 0; ev < num_evaluators; ++ev) {
          if (ev > 0) cout << '\t';
          cout << (boost::format("%s=%.6f") %
              evaluators2[ev]->getName() %
              scores2[ev]);
        }
        cout << endl; 
      }

      // Check winners and reset evaluators
      for (int ev = 0; ev < num_evaluators; ++ev) {
        if (scores1[ev] > scores2[ev]) {
          ++won[ev];
        }
      }
    }

    // Print final results
    if (!num_evaluators) {
      cout << "no evaluation metrics." << endl;
    } else {
      for (int ev = 0; ev < num_evaluators; ++ev) {
        if (ev > 0) cout << '\t';
        cout << (boost::format("%s: p=%.6f (%d/%d)") %
            evaluators1[ev]->getName() %
            (1.0 - won[ev] / (double)num_iteration) %
            won[ev] %
            num_iteration);
      }
      cout << endl;
    }

  } catch (exception & ex) {
    cerr << "ERROR: " << ex.what() << endl;
    return 1;
  }

  return 0;
}

