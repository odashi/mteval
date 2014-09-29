#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char * argv[]) {
    namespace OPT = boost::program_options;
    
    // parse options
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
        ("evaluator,e", OPT::value<string>(), "[required] evaluator name")
        ("sentence,s", "do sentence-wise evaluation");
        ;

    OPT::options_description opt;
    opt.add(opt_generic).add(opt_input).add(opt_config);

    OPT::variables_map argmap;
    try {
        OPT::store(OPT::parse_command_line(argc, argv, opt), argmap);
    } catch (exception & ex) {
        cerr << "ERROR: " << ex.what() << endl;
        cerr << "Usage: mteval [options] -e EVAL -r REF -h HYP" << endl;
        cerr << opt << endl;
        return 1;
    }
    OPT::notify(argmap);

    if (argmap.count("help") ||
        !argmap.count("reference") ||
        !argmap.count("hypothesis") ||
        !argmap.count("evaluator")) {
        cerr << "Usage: mteval [options] -e EVAL -r REF -h HYP" << endl;
        cerr << opt << endl;
        return 1;
    }

    string evaluator_name = argmap["evaluator"].as<string>();
    bool sentence_wise = !!argmap.count("sentence");

    cout << evaluator_name << endl;
    cout << sentence_wise << endl;

    return 0;
}

