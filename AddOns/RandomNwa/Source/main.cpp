// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* The simplest usage of the library.
 */

#include <boost/program_options.hpp>
#include <boost/random.hpp>

namespace po = boost::program_options;

#include <iostream>
#include <iterator>

using namespace std;


class incompatible_options_exception : public std::exception {
public:
    virtual const char* what() const throw() {
        return "Incompatibile options";
    }
};


int get_bernoulli(int n, double p)
{
    boost::mt19937 rng;
    boost::binomial_distribution<> bin(n, p);

    boost::variate_generator<boost::mt19937&, boost::binomial_distribution<> > die(rng, bin);
    return die();
}


int get_num_initial_states(po::variables_map const & options)
{
    if (options.count("number-of-initial-states") > 0) {
        if (options.count("initial-states-density") > 0) {
            throw incompatible_options_exception();
        }
        return options["number-of-initial-states"].as<int>();
    }
    else if (options.count("initial-states-density") > 0) {
        double density = options["initial-states-density"].as<int>() / 100.0;
        int states = options["number-of-states"].as<int>();
        return get_bernoulli(states, density);
    }
    else {
        return 1;
    }
}

int get_num_accepting_states(po::variables_map const & options)
{
    if (options.count("number-of-accepting-states") > 0) {
        if (options.count("accepting-states-density") > 0) {
            throw incompatible_options_exception();
        }
        return options["number-of-accepting-states"].as<int>();
    }
    else if (options.count("accepting-states-density") > 0) {
        double density = options["accepting-states-density"].as<int>() / 100.0;
        int states = options["number-of-states"].as<int>();
        return get_bernoulli(states, density);
    }
    else {
        return 1;
    }
}

int get_num_transitions_helper(po::variables_map const & options,
                               const char * absolute_density_name,
                               const char * deterministic_density_name)
{
    int states = options["number-of-states"].as<int>();
    int symbols = options["number-of-symbols"].as<int>();
    
    if (options.count(absolute_density_name) > 0) {
        if (options.count(deterministic_density_name) > 0) {
            throw incompatible_options_exception();
        }
        double density = options[absolute_density_name].as<int>() / 100.0;
        return get_bernoulli(states * states * symbols, density);
    }
    else if (options.count(deterministic_density_name) > 0) {
        double density = options[deterministic_density_name].as<int>() / 100.0;
        return get_bernoulli(states * symbols, density);
    }
    else {
        return get_bernoulli(states * states * symbols, 0.1);
    }
}


int get_num_call_transitions(po::variables_map const & options)
{
    return get_num_transitions_helper(options,
                                      "absolute-call-transition-density",
                                      "deterministic-call-transition-density");
}

int get_num_internal_transitions(po::variables_map const & options)
{
    return get_num_transitions_helper(options,
                                      "absolute-internal-transition-density",
                                      "deterministic-internal-transition-density");
}

int get_num_jump_transitions(po::variables_map const & options)
{
    return get_num_transitions_helper(options,
                                      "absolute-jump-transition-density",
                                      "deterministic-jump-transition-density");
}

int get_num_return_transitions(po::variables_map const & options)
{
    int states = options["number-of-states"].as<int>();
    int symbols = options["number-of-symbols"].as<int>();

    const char * absolute = "absolute-return-transition-density";
    const char * deterministic = "deterministic-return-transition-density";
    const char * single_src = "single-source-single-predecessor-return-transition-density";

    if (options.count(absolute) + options.count(deterministic)
        + options.count(single_src) > 1)
    {
        throw incompatible_options_exception();
    }
    
    if (options.count(absolute) > 0) {
        double density = options[absolute].as<int>() / 100.0;
        return get_bernoulli(states * states * states * symbols, density);
    }
    else if (options.count(deterministic) > 0) {
        double density = options[deterministic].as<int>() / 100.0;
        return get_bernoulli(states * states * symbols, density);
    }
    else if (options.count(single_src) > 0) {
        double density = options[single_src].as<int>() / 100.0;
        return get_bernoulli(states * symbols, density);
    }
    else {
        return get_bernoulli(states * states * symbols, 0.1);
    }
}


    

class NwaGenerator
{
    int num_states;
    int num_symbols;
    int num_initial_states;
    int num_accepting_states;

    int num_call;
    int num_internal; // no epsilon
    int num_epsilon;
    int num_return;

    NwaGenerator(po::variables_map const & options)
        : num_states(options["number-of-states"].as<int>())
        , num_symbols(options["number-of-symbols"].as<int>())
        , num_initial_states(get_num_initial_states(options))
        , num_accepting_states(get_num_accepting_states(options))
        , num_call(get_num_call_transitions(options))
        , num_internal(get_num_internal_transitions(options))
        , num_return(get_num_return_transitions(options))
    {}
};



int main(int ac, char* av[])
{
    try {

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")

            ("absolute-internal-transition-density", po::value<int>(), "")
            ("deterministic-internal-transition-density", po::value<int>(), "")
            ("absolute-jump-density", po::value<int>(), "")
            ("deterministic-jump-density", po::value<int>(), "")
            
            ("absolute-call-transition-density", po::value<int>(), "")
            ("deterministic-call-transition-density", po::value<int>(), "")

            ("absolute-return-transition-density", po::value<int>(), "")
            ("deterministic-return-transition-density", po::value<int>(), "")
            ("single-source-single-predecessor-return-transition-density", po::value<int>(), "")

            ("number-of-states", po::value<int>()->default_value(50), "")
            ("number-of-symbols", po::value<int>()->default_value(5), "")

            ("number-of-initial-states", po::value<int>(), "")
            ("number-of-accepting-states", po::value<int>(), "")
            ("initial-states-density", po::value<int>(), "")
            ("accepting-states-density", po::value<int>(), "")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }

        if (vm.count("compression")) {
            cout << "Compression level was set to " 
                 << vm["compression"].as<int>() << ".\n";
        } else {
            cout << "Compression level was not set.\n";
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}