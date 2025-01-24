#pragma once

#include "utils.hpp"
#include <utility>
#include <unordered_set>


class Graph
{
public:
    std::string folder; // the folder of datasets
    std::string graph;
    uint nu;  // the number of nodes in U
    uint nv;  // the number of nodes in V
    uint na;  // the number of attributes
    uint64 m; // the number of edges
    uint64 m_a;

    std::vector<std::vector<std::pair<uint, double> > > u_v_weight;
    std::vector<double> u_v_wsum;

    std::vector<std::vector<std::pair<uint, double> > > u_a_weight;
    std::vector<double> u_a_wsum;                                    

    std::vector<std::vector<std::pair<uint, double> > > v_u_weight; 
    std::vector<double> v_wsum;                                   

    std::vector<std::vector<std::pair<uint, double> > > a_u_weight; 
    std::vector<double> a_wsum;                                   

    std::vector<std::pair<uint, uint> > u_v_edges;
    std::vector<std::unordered_set<uint> > u_v_neighbors;
    std::vector<std::unordered_set<uint> > v_u_neighbors;

    std::vector<std::pair<uint, uint> > u_a_edges;
    std::vector<std::unordered_set<uint> > u_a_neighbors;
    std::vector<std::unordered_set<uint> > a_u_neighbors;

    void readState();
    void readGraph();
    void addUVEdge(uint u, uint v, double w);
    void addUAEdge(uint u, uint a, double w);
    std::string getGraphFolder() const;

    Graph(const std::string &t_folder, const std::string &t_graph);
};
