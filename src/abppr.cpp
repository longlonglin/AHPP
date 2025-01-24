#include "algo.hpp"
#include <boost/program_options.hpp>

using namespace std;

namespace po = boost::program_options;

namespace
{
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;
}

Config parseParams(int argc, char **argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("data-folder,f", po::value<string>()->required(), "graph data folder")
        ("graph-name,g", po::value<string>()->required(), "graph file name")
        ("algo,a", po::value<string>()->required(), "algorithm name")
        ("alpha", po::value<double>()->default_value(0.15), "alpha")
        ("beta", po::value<double>()->default_value(0.35), "beta")
        ("epsilon,e", po::value<double>()->default_value(1.0e-6), "epsilon");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm); // can throw
    po::notify(vm);

    Config config;

    if (vm.count("help"))
    {
        cout << desc << '\n';
        exit(0);
    }
    if (vm.count("data-folder"))
    {
        config.strFolder = vm["data-folder"].as<string>();
    }
    if (vm.count("graph-name"))
    {
        config.strGraph = vm["graph-name"].as<string>();
    }
    if (vm.count("algo"))
    {
        config.strAlgo = vm["algo"].as<string>();
    }
    if (vm.count("alpha"))
    {
        config.alpha = vm["alpha"].as<double>();
    }
    if (vm.count("beta"))
    {
        config.beta = vm["beta"].as<double>();
    }
    if (vm.count("epsilon"))
    {
        config.epsilon = vm["epsilon"].as<double>();
    }

    return config;
}

vector<int> loadSeed(const string &folder, const string &file_name, int count)
{
    FILE *fin = fopen((folder + "/" + file_name + "/seeds.txt").c_str(), "r");
    int s;
    vector<int> seeds;
    int i = 0;
    while (fscanf(fin, "%d", &s) != EOF)
    {
        seeds.emplace_back(s);
        if (++i >= count)
        {
            break;
        }
    }
    fclose(fin);
    cout << "read seeds Done!" << endl;
    return seeds;
}

int main(int argc, char **argv)
{
    Config config;
    try
    {
        config = parseParams(argc, argv);
        config.check();
    }
    catch (const exception &ex)
    {
        cerr << ex.what() << '\n';
        return ERROR_IN_COMMAND_LINE;
    }
    config.display();

    Graph graph(config.strFolder, config.strGraph);

    int query_count = 100;
    vector<int> seeds = loadSeed(config.strFolder, config.strGraph, query_count);
    int time_id = 0;

    if (config.strAlgo == APP)
    {
        time_id = 1;
        double rmax = config.epsilon / (graph.m + graph.m_a);
        cout << "start APP method!" << '\n';
        cout << "#epsilon: " << config.epsilon << '\n';
        cout << "#rmax: " << rmax << endl;
        vector<double> ppr;

        for (const auto &u : seeds)
        {
            APP_ALGO(u, config.alpha, config.beta, rmax, ppr, graph);
        }
    }

    else if (config.strAlgo == ASRP)
    {
        time_id = 2;
        double max_pr = computeMaxPr(config.alpha, config.beta, graph);
        double epsilon_f = config.epsilon / max_pr;
        cout << "start ASRP method!" << '\n';
        cout << "#epsilon: " << config.epsilon << '\n';
        cout << "#epsilon_f: " << epsilon_f << endl;
        vector<double> ppr;

        for (const auto &u : seeds)
        {
            ASRP_ALGO(u, config.alpha, config.beta, epsilon_f, max_pr, ppr, graph);
        }
    }

    // display time information
    cout << Timer::used(time_id) * 1000 / query_count << " milli-seconds per query" << endl;
    Timer::show();

    return SUCCESS;
}
