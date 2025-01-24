#include "graph.hpp"


void APP_ALGO(uint src, double alpha, double beta, double rmax, std::vector<double> &ppr, const Graph &graph);

void ASRP_ALGO(uint src, double alpha, double beta, double epsilon_f, double lamda, std::vector<double> &ppr, const Graph &graph);

double computeMaxPr(double alpha, double beta, const Graph &graph);