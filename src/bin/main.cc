#include <mteval/utils.h>
#include <mteval/Dictionary.h>
#include <mteval/Evaluator.h>
#include <mteval/EvaluatorFactory.h>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <vector>

using namespace std;
using namespace MTEval;

boost::program_options::variables_map parseOptions(int argc, char * argv []) {
    namespace OPT = boost::program_options;

    // set options

    OPT::options_description opt_generic("Generic options");
    opt_generic.add_options()
        ("help,?", "show this help and exit")
        ;
    OPT::options_description opt_input("Input files");
    opt_input.add_options()
        ("reference,r", OPT::value<string>(), "[required] reference file")
        ("hypothesis,h", OPT::value<string>(), "[required] hypothesis file")
        ;
    OPT::options_description opt_config("Configurations");
    opt_config.add_options()
        ("evaluator,e", OPT::value<string>(), "[required] evaluator names (e.x. \'BLEU,RIBES,WER\')")
        ("sentence,s", "do sentence-wise evaluation");
        ;
    OPT::options_description opt;
    opt.add(opt_generic).add(opt_input).add(opt_config);

    // parse

    OPT::variables_map args;
    try {
        OPT::store(OPT::parse_command_line(argc, argv, opt), args);
    } catch (exception & ex) {
        cerr << "ERROR: " << ex.what() << endl;
        exit(1);
    }
    OPT::notify(args);

    // process usage

    if (args.count("help")) {
        cerr << "Usage: mteval [options] -e EVAL1,EVAL2,... -r REF -h HYP" << endl;
        cerr << opt << endl;
        exit(1);
    }

    // check required options

    if (!args.count("reference") ||
        !args.count("hypothesis") ||
        !args.count("evaluator")) {
        cerr << "ERROR: insufficient required options" << endl;
        cerr << "(--help to show usage)" << endl;
        exit(1);
    }

    return move(args);
}

Sentence getSentence(const string & line, Dictionary & dict) {
    vector<string> word_list;
    string trimmed = boost::trim_copy(line);
    if (trimmed.empty()) return Sentence();
    boost::split(word_list, trimmed, boost::is_space(), boost::algorithm::token_compress_on);
    Sentence sent(word_list.size());
    transform(word_list.begin(), word_list.end(), sent.begin(), [&dict](const string & x) { return dict[x]; });
    return sent;
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

int main(int argc, char * argv[]) {
    
    try {

        auto args = parseOptions(argc, argv);

        vector<string> evaluator_names;
        boost::split(evaluator_names, args["evaluator"].as<string>(), boost::is_any_of(","));
        string filename_ref = args["reference"].as<string>();
        string filename_hyp = args["hypothesis"].as<string>();
        bool sentence_wise = !!args.count("sentence");

        // get evaluators

        vector<shared_ptr<Evaluator> > evaluators;
        for (const string & name : evaluator_names) {
            evaluators.push_back(EvaluatorFactory::create(name));
        }

        // open files
        unique_ptr<ifstream> ifs_ref = Utility::openInputStream(filename_ref);
        unique_ptr<ifstream> ifs_hyp = Utility::openInputStream(filename_hyp);

        Dictionary dict;
        string line_ref, line_hyp;

        // prepare evaluator

        while (getline(*ifs_ref, line_ref) && getline(*ifs_hyp, line_hyp)) {
            Sentence sent_ref = getSentence(line_ref, dict);
            Sentence sent_hyp = getSentence(line_hyp, dict);
            for (auto & ev : evaluators) {
                ev->prepare(sent_ref, sent_hyp);
            }
        }

        // analyze

        ifs_ref->clear();
        ifs_hyp->clear();
        ifs_ref->seekg(0, ios::beg);
        ifs_hyp->seekg(0, ios::beg);

        while (getline(*ifs_ref, line_ref) && getline(*ifs_hyp, line_hyp)) {
            Sentence sent_ref = getSentence(line_ref, dict);
            Sentence sent_hyp = getSentence(line_hyp, dict);
            for (auto & ev : evaluators) {
                ev->calculate(sent_ref, sent_hyp);
            }
            if (sentence_wise) {
                // print sentence-wise scores
                printScores(evaluators);
                for (auto & ev : evaluators) {
                    ev->resetCumulative();
                }
            }
        }

        if (!sentence_wise) {
            // print corpus scores
            printScores(evaluators);
        }

    } catch (exception & ex) {
        cerr << "ERROR: " << ex.what() << endl;
        return 1;
    }

    return 0;
}

