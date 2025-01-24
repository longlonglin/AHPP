#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

typedef unsigned int uint;
typedef unsigned long long uint64;

#ifndef TIMES_PER_SEC
#define TIMES_PER_SEC (1.0e9)
#endif

const std::string APP = "app";
const std::string ASRP = "asrp";

struct Config
{
    std::string strFolder;
    std::string strGraph;
    std::string strAlgo;
    double alpha;
    double beta;
    double epsilon;

    void display()
    {
        std::cout << "====================Configurations==================" << '\n';
        std::cout << "data folder: " << strFolder << '\n';
        std::cout << "graph file name: " << strGraph << '\n';
        std::cout << "algorithm: " << strAlgo << '\n';
        std::cout << "alpha: " << alpha << '\n';
        std::cout << "beta: " << beta << '\n';
        std::cout << "absolute/relative error epsilon: " << epsilon << '\n';
        std::cout << "====================Configurations==================" << std::endl;
    }

    double logBase(double base, double value)
    {
        return log(value) / log(base);
    }

    void check()
    {
        std::vector<std::string> Algos = {APP, ASRP};
        auto f = std::find(Algos.begin(), Algos.end(), strAlgo);
        assert(f != Algos.end());
    }
};

class Timer
{
public:
    static std::vector<double> timeUsed;
    static std::vector<std::string> timeUsedDesc;
    int id;
    std::chrono::steady_clock::time_point startTime;
    bool showOnDestroy;

    Timer(int id, std::string desc = "", bool showOnDestroy = false)
    {
        this->id = id;
        while ((int)timeUsed.size() <= id)
        {
            timeUsed.emplace_back(0.0);
            timeUsedDesc.emplace_back("");
        }
        timeUsedDesc[id] = desc;
        this->showOnDestroy = showOnDestroy;
        startTime = std::chrono::steady_clock::now();
    }

    static double used(int id)
    {
        return timeUsed[id] / TIMES_PER_SEC;
    }

    ~Timer()
    {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - startTime).count();
        if (showOnDestroy)
        {
            std::cout << "time spent on " << timeUsedDesc[id] << ": " << duration / TIMES_PER_SEC << "s" << std::endl;
        }
        timeUsed[id] += duration;
    }

    static void show(bool debug = false)
    {
        std::cout << "####### Timer #######" << std::endl;
        for (int i = 0; i < (int)timeUsed.size(); i++)
        {
            if (timeUsed[i] > 0)
            {
                char str[100];
                sprintf(str, "%.6lf", timeUsed[i] / TIMES_PER_SEC);
                std::string s = str;
                if ((int)s.size() < 15)
                    s = " " + s;
                char t[100];
                memset(t, 0, sizeof t);
                sprintf(t, "%4d   %s  %s", i, s.c_str(), timeUsedDesc[i].c_str());
                std::cout << t << std::endl;
            }
        }
    }

    static void reset(int id)
    {
        if (id >= 0 && id < timeUsed.size())
        {
            timeUsed[id] = 0;
        }
    }

    static void clearAll()
    {
        timeUsed.clear();
        timeUsedDesc.clear();
    }
};