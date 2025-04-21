#include "UnionFind.h"

namespace GraphLib {
    UnionFind::UnionFind(const int n) : find_calls(0) {
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

    int UnionFind::get_find_calls() const {
        return find_calls;
    }

    int UnionFind::find(const int node) {
        find_calls++;
        if (node == parent[node]) {
            return node;
        }
        return find(parent[node]);
    }

    int UnionFind::compress_find(const int node) {
        find_calls++;
        if (node != parent[node]) {
            int root = compress_find(parent[node]);
            if (root != parent[node]) {
                parent[node] = root;
            }
        }
        return parent[node];
    }

    void UnionFind::unite(const int root_x, const int root_y) {
        parent[root_x] = root_y;
    }

    void UnionFind::rank_unite(const int root_x, const int root_y) {
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
