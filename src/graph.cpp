#include "graph.hpp"
#include <fstream>
#include <sstream>

#define ASSERT(v)                                                            \
    {                                                                        \
        if (!(v))                                                            \
        {                                                                    \
            cerr << "ASSERT FAIL @ " << __FILE__ << ":" << __LINE__ << endl; \
            exit(1);                                                         \
        }                                                                    \
    }

using namespace std;

void handle_error(const char *msg)
{
    perror(msg);
    exit(255);
}

void Graph::readState()
{
    ifstream fin((folder + "/" + graph + "/stat.txt").c_str());
    string s;
    if (fin.is_open())
    {
        while (fin >> s)
        {
            string prefix_string = s.substr(0, 2), postfix_string = s.substr(2);
            if (prefix_string == "u=")
            {
                this->nu = atoi(postfix_string.c_str());
            }
            else if (prefix_string == "v=")
            {
                this->nv = atoi(postfix_string.c_str());
            }
            else if (prefix_string == "m=")
            {
                this->m = atoi(postfix_string.c_str());
            }
            else if (prefix_string == "a=")
            {
                this->na = atoi(postfix_string.c_str());
            }
        }
        fin.close();
    }
    else
    {
        handle_error("Fail to open graph state file!");
    }
}

void Graph::readGraph()
{
    FILE *fin = fopen((folder + "/" + graph + "/Edge.txt").c_str(), "r");
    uint64 read_edge_cnt = 0;
    uint u, v;
    double w;
    while (fscanf(fin, "%u %u %lf", &u, &v, &w) != EOF)
    {
        read_edge_cnt++;
        ASSERT(u < nu);
        ASSERT(v < nv);
        addUVEdge(u, v, w);
    }
    fclose(fin);
    ASSERT(read_edge_cnt == m);
    cout << "read edges done!" << endl;

    fin = fopen((folder + "/" + graph + "/Attribute_U.txt").c_str(), "r");
    read_edge_cnt = 0;
    while (fscanf(fin, "%u %u %lf", &u, &v, &w) != EOF)
    {
        ASSERT(u < nu);
        ASSERT(v < na);
        addUAEdge(u, v, w);
        read_edge_cnt++;
    }
    fclose(fin);
    m_a = read_edge_cnt;

    cout << "read graph done!" << endl;
}

void Graph::addUAEdge(uint u, uint a, double w)
{
    u_a_edges.emplace_back(u, a);
    u_a_neighbors[u].insert(a);
    a_u_neighbors[a].insert(u);

    u_a_weight[u].emplace_back(a, w);
    a_u_weight[a].emplace_back(u, w);
    u_a_wsum[u] += w;
    a_wsum[a] += w;
}

void Graph::addUVEdge(uint u, uint v, double w)
{
    u_v_edges.emplace_back(u, v);
    u_v_neighbors[u].insert(v);
    v_u_neighbors[v].insert(u);

    u_v_weight[u].emplace_back(v, w);
    v_u_weight[v].emplace_back(u, w);
    u_v_wsum[u] += w;
    v_wsum[v] += w;
}

Graph::Graph(const string &t_folder, const string &t_graph) : folder(t_folder), graph(t_graph)
{
    readState();
    cout << "graph info: nu = " << nu << ", nv = " << nv << ", na = " << na << ", m = " << m << endl;

    u_v_weight = vector<vector<pair<uint, double> > >(nu);
    u_a_weight = vector<vector<pair<uint, double> > >(nu);
    u_v_wsum = vector<double>(nu, 0.0);
    u_a_wsum = vector<double>(nu, 0.0);

    v_u_weight = vector<vector<pair<uint, double> > >(nv);
    v_wsum = vector<double>(nv, 0.0);

    a_u_weight = vector<vector<pair<uint, double> > >(na);
    a_wsum = vector<double>(na, 0.0);

    u_v_neighbors = vector<unordered_set<uint> >(nu);
    v_u_neighbors = vector<unordered_set<uint> >(nv);

    u_a_neighbors = vector<unordered_set<uint> >(nu);
    a_u_neighbors = vector<unordered_set<uint> >(na);

    cout << "loading graph..." << endl;
    readGraph();
}

string Graph::getGraphFolder() const
{
    return string(folder + "/" + graph + "/");
}
