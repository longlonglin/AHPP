#include "algo.hpp"
#include <queue>
#include <random>

using namespace std;


void APP_ALGO(uint src, double alpha, double beta, double rmax, vector<double> &ppr, const Graph &graph)
{
    uint nu = graph.nu, nv = graph.nv, na = graph.na;
    vector<double> vecUResidue(nu, 0.0);
    vector<double> vecVResidue(nv, 0.0);
    vector<double> vecAResidue(na, 0.0);
    ppr = vector<double>(nu, 0.0);
    queue<uint> Qu;
    queue<uint> Qv;
    queue<uint> Qa;

    vecUResidue[src] = 1;
    Qu.emplace(src);

    double residue, mass, threhold;

    Timer tm(1, "APP");
    while (!Qu.empty())
    {
        while (!Qu.empty())
        {
            uint u = Qu.front(); Qu.pop();
            residue = vecUResidue[u];
			vecUResidue[u] = 0;
            if (graph.u_a_neighbors[u].empty() && graph.u_v_neighbors[u].empty())
            {
                ppr[u] += residue;
            }
            else
            {
                ppr[u] += alpha * residue;
                residue = (1.0 - alpha) * residue;

                double v_scale = 1.0 - beta, a_scale = beta;
                if (graph.u_v_neighbors[u].empty())
                {
                    a_scale = 1;
                }
                if (graph.u_a_neighbors[u].empty())
                {
                    v_scale = 1;
                }

                for (const auto &[v, w] : graph.u_v_weight[u])
                {
                    if (vecVResidue[v] == 0)
                    {
                        Qv.emplace(v);
                    }
                    vecVResidue[v] += v_scale * residue * w / graph.u_v_wsum[u];
                }

                for (const auto &[a, w] : graph.u_a_weight[u])
                {
                    if (vecAResidue[a] == 0)
                    {
                        Qa.emplace(a);
                    }
                    vecAResidue[a] += a_scale * residue * w / graph.u_a_wsum[u];
                }
            }
        }

        while (!Qv.empty())
        {
            uint v = Qv.front(); Qv.pop();
            residue = vecVResidue[v];
			vecVResidue[v] = 0;
            if (graph.v_u_neighbors[v].empty()) 
            {
                continue;
            }
            for (const auto &[u, w] : graph.v_u_weight[v])
            {
                mass = residue * w / graph.v_wsum[v];
                threhold = rmax * ((int)graph.u_v_neighbors[u].size() + (int)graph.u_a_neighbors[u].size());
                if (vecUResidue[u] <= threhold && vecUResidue[u] + mass > threhold)
                {
                    Qu.emplace(u);
                }
                vecUResidue[u] += mass;
            }
        }

        while (!Qa.empty())
        {
            uint a = Qa.front(); Qa.pop();
            residue = vecAResidue[a];
            vecAResidue[a] = 0;
            if (graph.a_u_neighbors[a].empty()) 
            {
                continue;
            }
            for (const auto &[u, w] : graph.a_u_weight[a])
            {
                mass = residue * w / graph.a_wsum[a];
                threhold = rmax * ((int)graph.u_v_neighbors[u].size() + (int)graph.u_a_neighbors[u].size());
                if (vecUResidue[u] <= threhold &&  vecUResidue[u] + mass > threhold)
                {
                    Qu.emplace(u);
                }
                vecUResidue[u] += mass;
            }
        }
    }
}


void ASRP_ALGO(uint src, double alpha, double beta, double epsilon_f, double lamda, std::vector<double> &ppr, const Graph &graph)
{
    uint nu = graph.nu, nv = graph.nv, na = graph.na;
    vector<double> vecUResidue(nu, 0.0);
    vector<double> vecVResidue(nv, 0.0);
    vector<double> vecAResidue(na, 0.0);
    ppr = vector<double>(nu, 0.0);
    queue<uint> Qu;
    queue<uint> Qv;
    queue<uint> Qa;

    vecUResidue[src] = 1;
    Qu.emplace(src);

    double residue, mass, threhold, max_r;
    uint64 cost = 0;

    Timer tm(2, "ASRP");
    while (!Qu.empty())
    {
        max_r = 0;
        while (!Qu.empty())
        {
            uint u = Qu.front(); Qu.pop();
            residue = vecUResidue[u];
			vecUResidue[u] = 0;
            if (graph.u_a_neighbors[u].empty() && graph.u_v_neighbors[u].empty())
            {
                ppr[u] += residue;
            }
            else
            {
                ppr[u] += alpha * residue;
                residue = (1.0 - alpha) * residue;
                cost += (uint)graph.u_v_neighbors[u].size() + (uint)graph.u_a_neighbors[u].size();

                double v_scale = 1.0 - beta, a_scale = beta;
                if (graph.u_v_neighbors[u].empty())
                {
                    a_scale = 1;
                }
                if (graph.u_a_neighbors[u].empty())
                {
                    v_scale = 1;
                }

                for (const auto &[v, w] : graph.u_v_weight[u])
                {
                    if (vecVResidue[v] == 0)
                    {
                        Qv.emplace(v);
                    }
                    vecVResidue[v] += v_scale * residue * w / graph.u_v_wsum[u];
                }

                for (const auto &[a, w] : graph.u_a_weight[u])
                {
                    if (vecAResidue[a] == 0)
                    {
                        Qa.emplace(a);
                    }
                    vecAResidue[a] += a_scale * residue * w / graph.u_a_wsum[u];
                }
            }
        }

        while (!Qv.empty())
        {
            uint v = Qv.front(); Qv.pop();
            residue = vecVResidue[v];
			vecVResidue[v] = 0;
            if (graph.v_u_neighbors[v].empty()) 
            {
                continue;
            }
            cost += (uint)graph.v_u_neighbors[v].size();
            for (const auto &[u, w] : graph.v_u_weight[v])
            {
                mass = residue * w / graph.v_wsum[v];
                if (vecUResidue[u] <= epsilon_f && vecUResidue[u] + mass > epsilon_f)
                {
                    Qu.emplace(u);
                }
                vecUResidue[u] += mass;
                max_r = max(max_r, vecUResidue[u]);
            }
        }

        while (!Qa.empty())
        {
            uint a = Qa.front(); Qa.pop();
            residue = vecAResidue[a];
            vecAResidue[a] = 0;
            if (graph.a_u_neighbors[a].empty()) 
            {
                continue;
            }
            cost += (uint)graph.a_u_neighbors[a].size();
            for (const auto &[u, w] : graph.a_u_weight[a])
            {
                mass = residue * w / graph.a_wsum[a];
                if (vecUResidue[u] <= epsilon_f && vecUResidue[u] + mass > epsilon_f)
                {
                    Qu.emplace(u);
                }
                vecUResidue[u] += mass;
                max_r = max(max_r, vecUResidue[u]);
            }
        }

        threhold = 2.0 * (graph.m + graph.m_a) * log(1.0 / max_r / lamda) / log(1.0 / (1.0 - alpha));
        if (cost >= threhold)
        {
            break;
        }
    }

    uint cnt = (uint)Qu.size();
    while (cnt > 0)
    {
        cnt = 0;
        while (!Qu.empty())
        {
            uint u = Qu.front(); Qu.pop();
            residue = vecUResidue[u];
			vecUResidue[u] = 0;
            if (graph.u_a_neighbors[u].empty() && graph.u_v_neighbors[u].empty())
            {
                ppr[u] += residue;
            }
            else
            {
                ppr[u] += alpha * residue;
                residue = (1.0 - alpha) * residue;

                double v_scale = 1.0 - beta, a_scale = beta;
                if (graph.u_v_neighbors[u].empty())
                {
                    a_scale = 1;
                }
                if (graph.u_a_neighbors[u].empty())
                {
                    v_scale = 1;
                }

                for (const auto &[v, w] : graph.u_v_weight[u])
                {
                    if (vecVResidue[v] == 0)
                    {
                        Qv.emplace(v);
                    }
                    vecVResidue[v] += v_scale * residue * w / graph.u_v_wsum[u];
                }

                for (const auto &[a, w] : graph.u_a_weight[u])
                {
                    if (vecAResidue[a] == 0)
                    {
                        Qa.emplace(a);
                    }
                    vecAResidue[a] += a_scale * residue * w / graph.u_a_wsum[u];
                }
            }
        }

        while (!Qv.empty())
        {
            uint v = Qv.front(); Qv.pop();
            residue = vecVResidue[v];
			vecVResidue[v] = 0;
            if (graph.v_u_neighbors[v].empty()) 
            {
                continue;
            }
            for (const auto &[u, w] : graph.v_u_weight[v])
            {
                if (vecUResidue[u] == 0)
                {
                    Qu.emplace(u);
                }
                mass = residue * w / graph.v_wsum[v];
                if (vecUResidue[u] <= epsilon_f && vecUResidue[u] + mass > epsilon_f)
                {
                    cnt++;
                }
                vecUResidue[u] += mass;
            }
        }

        while (!Qa.empty())
        {
            uint a = Qa.front(); Qa.pop();
            residue = vecAResidue[a];
            vecAResidue[a] = 0;
            if (graph.a_u_neighbors[a].empty()) 
            {
                continue;
            }
            for (const auto &[u, w] : graph.a_u_weight[a])
            {
                if (vecUResidue[u] == 0)
                {
                    Qu.emplace(u);
                }
                mass = residue * w / graph.a_wsum[a];
                if (vecUResidue[u] <= epsilon_f && vecUResidue[u] + mass > epsilon_f)
                {
                    cnt++;
                }
                vecUResidue[u] += mass;
            }
        }
    }
}

double computeMaxPr(double alpha, double beta, const Graph &graph)
{
    uint nu = graph.nu, nv = graph.nv, na = graph.na;
    vector<double> vecUResidue(nu, 0.0);
    vector<double> vecVResidue(nv, 0.0);
    vector<double> vecAResidue(na, 0.0);
    vector<double> ppr(nu, 0.0);
    queue<uint> Qu;
    queue<uint> Qv;
    queue<uint> Qa;

    for (uint u = 0; u < nu; u++)
    {
        vecUResidue[u] = 1;
        Qu.emplace(u);
    }

    uint n_iter = 160;
    double residue;

    for (uint i = 0; i < n_iter + 1; i++)
    {
        while (!Qu.empty())
        {
            uint u = Qu.front(); Qu.pop();
            residue = vecUResidue[u];
			vecUResidue[u] = 0;
            if (graph.u_a_neighbors[u].empty() && graph.u_v_neighbors[u].empty())
            {
                ppr[u] += residue;
            }
            else
            {
                ppr[u] += alpha * residue;
                residue = (1.0 - alpha) * residue;

                double v_scale = 1.0 - beta, a_scale = beta;
                if (graph.u_v_neighbors[u].empty())
                {
                    a_scale = 1;
                }
                if (graph.u_a_neighbors[u].empty())
                {
                    v_scale = 1;
                }

                for (const auto &[v, w] : graph.u_v_weight[u])
                {
                    if (vecVResidue[v] == 0)
                    {
                        Qv.emplace(v);
                    }
                    vecVResidue[v] += v_scale * residue * w / graph.u_v_wsum[u];
                }

                for (const auto &[a, w] : graph.u_a_weight[u])
                {
                    if (vecAResidue[a] == 0)
                    {
                        Qa.emplace(a);
                    }
                    vecAResidue[a] += a_scale * residue * w / graph.u_a_wsum[u];
                }
            }
        }

        while (!Qv.empty())
        {
            uint v = Qv.front(); Qv.pop();
            residue = vecVResidue[v];
			vecVResidue[v] = 0;
            if (graph.v_u_neighbors[v].empty()) 
            {
                continue;
            }
            for (const auto &[u, w] : graph.v_u_weight[v])
            {
                if (vecUResidue[u] == 0)
                {
                    Qu.emplace(u);
                }
                vecUResidue[u] += residue * w / graph.v_wsum[v];
            }
        }

        while (!Qa.empty())
        {
            uint a = Qa.front(); Qa.pop();
            residue = vecAResidue[a];
            vecAResidue[a] = 0;
            if (graph.a_u_neighbors[a].empty()) 
            {
                continue;
            }
            for (const auto &[u, w] : graph.a_u_weight[a])
            {
                if (vecUResidue[u] == 0)
                {
                    Qu.emplace(u);
                }
                vecUResidue[u] += residue * w / graph.a_wsum[a];
            }
        }
    }

    return *max_element(ppr.begin(), ppr.end()) + nu * pow(1 - alpha, n_iter + 1);
}