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
        ("help,?", "show this help and exit")
        ("verbose,v", "show intermediate results")
        ;
    OPT::options_description opt_input("Input files");
    opt_input.add_options()
        ("reference,r", OPT::value<string>(), "(required) reference file")
        ("hypothesis,h", OPT::value<vector<string> >()->multitoken(), "(required) 2 hypothesis file")
        ;
    OPT::options_description opt_config("Configurations");
    opt_config.add_options()
        ("evaluator,e", OPT::value<vector<string> >()->multitoken(), "(required) evaluator descriptions")
        ("num-iteration,i", OPT::value<int>(), "(required) number of virtual test sets to evaluate")
        ("num-sample,s", OPT::value<int>(), "(required) number of sentences in a virtual test sets")
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
        cerr << "Usage: " << binname << " [options] -i INT -s INT -e EVAL1 EVAL2 ... -r REF -h HYP1 HYP2" << endl;
        cerr << opt << endl;
        exit(1);
    }

    // check required options

    if (!args.count("reference") ||
        !args.count("hypothesis") ||
        !args.count("evaluator") ||
        !args.count("num-iteration") ||
        !args.count("num-sample")) {
        cerr << "ERROR: insufficient required options" << endl;
        cerr << "(--help to show usage)" << endl;
        exit(1);
    }

    return move(args);
}

void printScores(const vector<shared_ptr<Evaluator> > & evaluators) {
    if (evaluators.empty()) {
        cout << "no evaluation metrics." << endl;
        return;
    }
    for (size_t i = 0; i < evaluators.size(); ++i) {
        if (i > 0) cout << '\t';
        cout << (boost::format("%s=%.6f") % evaluators[i]->getName() % evaluators[i]->getCumulative());
    }
    cout << endl;
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

        vector<string> evaluator_desc = args["evaluator"].as<vector<string> >();
        int num_iteration = args["num-iteration"].as<int>();
        int num_sample = args["num-sample"].as<int>();
        string filename_ref = args["reference"].as<string>();
        vector<string> filename_hyp = args["hypothesis"].as<vector<string> >();

        // argument validation
        if (num_iteration <= 0) throw runtime_error("-i must be greater than 0");
        if (num_sample <= 0) throw runtime_error("-s must be greater than 0");
        if (filename_hyp.size() != 2) throw runtime_error("-h must be set just 2 files");

        // get evaluators
        int num_evaluators = evaluator_desc.size();
        vector<shared_ptr<Evaluator> > evaluators1, evaluators2;
        for (const string & desc : evaluator_desc) {
            evaluators1.push_back(EvaluatorFactory::create(desc));
            evaluators2.push_back(EvaluatorFactory::create(desc));
        }

        Dictionary dict;

        // load corpus
        Corpus corpus_ref = loadCorpus(filename_ref, dict);
        Corpus corpus_hyp1 = loadCorpus(filename_hyp[0], dict);
        Corpus corpus_hyp2 = loadCorpus(filename_hyp[1], dict);

        size_t num_sentences = corpus_ref.size();
        if (corpus_hyp1.size() != num_sentences || corpus_hyp2.size() != num_sentences) {
            throw runtime_error("corpus size are different");
        }

        // number of winner (hyp1 > hyp2)
        vector<int> won(evaluators1.size(), 0);

        // randomizer
        random_device devrand;
        mt19937 mtrand(devrand());
        uniform_int_distribution<int> dist(0, num_sentences - 1);
        auto next = [&](){ return dist(mtrand); };

        for (int iteration = 0; iteration < num_iteration; ++iteration) {

            // choose sentences and prepare evaluators
            vector<int> ids(num_sample);
            for (int sample = 0; sample < num_sample; ++sample) {
                int id = next();
                ids[sample] = id;
                for (auto & ev : evaluators1) ev->prepare(corpus_ref[id], corpus_hyp1[id]);
                for (auto & ev : evaluators2) ev->prepare(corpus_ref[id], corpus_hyp2[id]);
            }

            // analyze
            for (int id : ids) {
                for (auto & ev : evaluators1) ev->calculate(corpus_ref[id], corpus_hyp1[id]);
                for (auto & ev : evaluators2) ev->calculate(corpus_ref[id], corpus_hyp2[id]);
            }

            // print results
            if (verbose) {
                cout << "Iteration " << (iteration + 1) << ":" << endl;
                printScores(evaluators1);
                printScores(evaluators2);
            }

            // check winners and reset evaluators
            for (int ev = 0; ev < num_evaluators; ++ev) {
                if (evaluators1[ev]->getCumulative() > evaluators2[ev]->getCumulative()) {
                    ++won[ev];
                }
                evaluators1[ev]->resetCumulative();
                evaluators2[ev]->resetCumulative();
            }
        }

        // print final results
        if (!num_evaluators) {
            cout << "no evaluation metrics." << endl;
        } else {
            for (int ev = 0; ev < num_evaluators; ++ev) {
                if (ev > 0) cout << '\t';
                cout << (boost::format("%s: p=%.6f (%d/%d)")
                    % evaluators1[ev]->getName()
                    % (1.0 - won[ev] / (double)num_iteration)
                    % won[ev]
                    % num_iteration);
            }
            cout << endl;
        }

    } catch (exception & ex) {
        cerr << "ERROR: " << ex.what() << endl;
        return 1;
    }

    return 0;
}

