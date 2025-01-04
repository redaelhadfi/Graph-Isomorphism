#include "graph_utils.h"
#include <algorithm>

// Function definition
bool areGraphsIsomorphic(const Graph& g1, const Graph& g2) {
    if (g1.numVertices != g2.numVertices) return false;

    int n = g1.numVertices;
    std::vector<int> perm(n);
    for (int i = 0; i < n; ++i) perm[i] = i;

    do {
        bool match = true;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (g1.adjacencyMatrix[i][j] != g2.adjacencyMatrix[perm[i]][perm[j]]) {
                    match = false;
                    break;
                }
            }
            if (!match) break;
        }
        if (match) return true;

    } while (std::next_permutation(perm.begin(), perm.end()));

    return false;
}
