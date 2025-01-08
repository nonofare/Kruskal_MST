#include "UnionFind.h"

namespace GraphLib {
    UnionFind::UnionFind(const int n) {
        parent = new int[n];
        rank = new int[n]{};
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    UnionFind::~UnionFind() {
        delete[] parent;
        delete[] rank;
    }

    int UnionFind::Find(int node) {
        while (node != parent[node]) {
            node = parent[node];
        }
        return node;
    }

    int UnionFind::CompressFind(const int node) {
        if (node != parent[node]) {
            parent[node] = CompressFind(parent[node]);
        }
        return parent[node];
    }

    void UnionFind::Unite(const int node_x, const int node_y) {
        int root_x = CompressFind(node_x);
        int root_y = CompressFind(node_y);

        if (root_x != root_y) {
            parent[root_x] = root_y;
        }
    }

    void UnionFind::RankUnite(const int node_x, const int node_y) {
        int root_x = CompressFind(node_x);
        int root_y = CompressFind(node_y);

        if (root_x != root_y) {
            if (rank[root_x] < rank[root_y]) {
                parent[root_x] = root_y;
            } else if (rank[root_x] > rank[root_y]) {
                parent[root_y] = root_x;
            } else {
                parent[root_y] = root_x;
                rank[root_x]++;
            }
        }
    }
}
