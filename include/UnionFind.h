#ifndef UNIONFIND_H
#define UNIONFIND_H

namespace GraphLib {
    class UnionFind {
        int *parent;
        int *rank;

        int find_calls;

    public:
        explicit UnionFind(int n);

        ~UnionFind();

        int get_find_calls() const;

        int find(int node);

        int compress_find(int node);

        void unite(int root_x, int root_y);

        void rank_unite(int root_x, int root_y);
    };
}

#endif
