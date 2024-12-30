#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <vector>
#include <string>

// Graph structure
struct Graph {
    int numVertices;
    std::vector<std::vector<int>> adjacencyMatrix;
};

// Function declarations
Graph loadGraphFromFile(const std::string& filename);
bool areGraphsIsomorphic(const Graph& g1, const Graph& g2);

#endif // GRAPH_UTILS_H
