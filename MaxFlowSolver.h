#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

struct Edge {
    int from;
    int to;
    long long cap;
    long long flow;

    Edge(int f = -1, int t = -1, long long c = 0);
};

class MaxFlowSolver {
private:
    int n;
    std::vector<std::vector<long long>> capacity;
    std::vector<std::vector<long long>> originalCap;
    std::vector<std::vector<int>> adj;
    std::vector<Edge> edges;

    long long bfs(int s, int t, std::vector<int>& parent);

public:
    explicit MaxFlowSolver(int vertices);
    bool addEdge(int from, int to, long long cap);
    long long getMaxFlow(int s, int t);
    void printFlow(std::ostream& out = std::cout) const;
    void printAdjacencyMatrix(std::ostream& out = std::cout) const;
    int getVertexCount() const;
    int getEdgeCount() const;
};

bool loadFromFile(const std::string& filename, MaxFlowSolver& solver, int& s, int& t);