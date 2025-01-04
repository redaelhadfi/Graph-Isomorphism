#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <vector>

// Graph structure
struct Graph {
    int numVertices = 0;
    std::vector<std::vector<int>> adjacencyMatrix;
};

// Function declaration for isomorphism check
bool areGraphsIsomorphic(const Graph& g1, const Graph& g2);

#endif // GRAPH_UTILS_H
