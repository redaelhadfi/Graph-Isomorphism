#include "graph_utils.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

// Load a graph from a file
// Load a graph from a file
Graph loadGraphFromFile(const std::string& filename) {
    Graph graph;
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // Read the number of vertices
    file >> graph.numVertices;
    if (graph.numVertices <= 0) {
        throw std::runtime_error("Invalid number of vertices in: " + filename);
    }

    // Initialize the adjacency matrix
    graph.adjacencyMatrix.resize(graph.numVertices, std::vector<int>(graph.numVertices));

    // Read the adjacency matrix
    for (int i = 0; i < graph.numVertices; ++i) {
        for (int j = 0; j < graph.numVertices; ++j) {
            if (!(file >> graph.adjacencyMatrix[i][j])) {
                throw std::runtime_error("Invalid file format in: " + filename);
            }
            // Validate matrix values (only 0 or 1 allowed)
            if (graph.adjacencyMatrix[i][j] != 0 && graph.adjacencyMatrix[i][j] != 1) {
                throw std::runtime_error("Adjacency matrix must contain only 0s and 1s in: " + filename);
            }
        }
    }

    // Debugging: Print the loaded adjacency matrix
    std::cout << "Loaded adjacency matrix from " << filename << ":\n";
    for (const auto& row : graph.adjacencyMatrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    // Validate matrix symmetry
    for (int i = 0; i < graph.numVertices; ++i) {
        for (int j = 0; j < graph.numVertices; ++j) {
            if (graph.adjacencyMatrix[i][j] != graph.adjacencyMatrix[j][i]) {
                throw std::runtime_error("Adjacency matrix is not symmetric in: " + filename);
            }
        }
    }

    return graph;
}


bool areGraphsIsomorphic(const Graph& g1, const Graph& g2) {
    if (g1.numVertices != g2.numVertices) {
        std::cout << "Graph sizes are different. Not isomorphic.\n";
        return false;
    }

    int n = g1.numVertices;
    std::vector<int> perm(n);
    for (int i = 0; i < n; ++i) perm[i] = i;

    int totalPermutations = 0;
    do {
        ++totalPermutations;
        std::cout << "Testing permutation #" << totalPermutations << ": ";
        for (int v : perm) std::cout << v << " ";
        std::cout << "\n";

        bool isIsomorphic = true;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int mappedI = perm[i];
                int mappedJ = perm[j];

                std::cout << "Comparing g1[" << i << "][" << j << "] = " << g1.adjacencyMatrix[i][j]
                          << " with g2[" << mappedI << "][" << mappedJ << "] = " << g2.adjacencyMatrix[mappedI][mappedJ]
                          << "\n";

                if (g1.adjacencyMatrix[i][j] != g2.adjacencyMatrix[mappedI][mappedJ]) {
                    std::cout << "Mismatch at g1[" << i << "][" << j << "] = " << g1.adjacencyMatrix[i][j]
                              << " != g2[" << mappedI << "][" << mappedJ << "] = " << g2.adjacencyMatrix[mappedI][mappedJ]
                              << "\n";
                    isIsomorphic = false;
                    break;
                }
            }
            if (!isIsomorphic) break;
        }

        if (isIsomorphic) {
            std::cout << "Graphs are isomorphic under this permutation: ";
            for (int v : perm) std::cout << v << " ";
            std::cout << "\n";
            return true;
        }

    } while (std::next_permutation(perm.begin(), perm.end()));

    std::cout << "Total permutations tested: " << totalPermutations << "\n";
    std::cout << "No matching permutation found. Graphs are not isomorphic.\n";
    return false;
}
