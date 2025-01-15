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

    int UnionFind::GetFindCalls() const {
        return find_calls;
    }

    int UnionFind::Find(int node) {
        find_calls++;
        if (node == parent[node]) {
            return node;
        }
        return Find(parent[node]);
    }

    int UnionFind::CompressFind(const int node) {
        find_calls++;
        if (node != parent[node]) {
            int root = CompressFind(parent[node]);
            if (root != parent[node]) {
                parent[node] = root;
            }
        }
        return parent[node];
    }

    void UnionFind::Unite(const int root_x, const int root_y) {
        parent[root_x] = root_y;
    }

    void UnionFind::RankUnite(const int root_x, const int root_y) {
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
