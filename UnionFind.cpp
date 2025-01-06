#include "UnionFind.h"

namespace GraphLib {
    UnionFind::UnionFind(const int n) {
        parent = new int[n];
        rank = new int[n]{0};
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    UnionFind::~UnionFind() {
        delete[] parent;
        delete[] rank;
    }

    int UnionFind::SimpleFind(int x) {
        while (x != parent[x]) {
            x = parent[x];
        }
        return x;
    }

    int UnionFind::CompressFind(const int x) {
        if (x != parent[x]) {
            parent[x] = CompressFind(parent[x]);
        }
        return parent[x];
    }

    void UnionFind::SimpleUnite(const int x, const int y) {
        int root_x = CompressFind(x);
        int root_y = CompressFind(y);

        if (root_x != root_y) {
            parent[root_x] = root_y;
        }
    }

    void UnionFind::RankUnite(const int x, const int y) {
        int root_x = CompressFind(x);
        int root_y = CompressFind(y);

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
