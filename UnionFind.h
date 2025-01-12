#ifndef UNIONFIND_H
#define UNIONFIND_H

namespace GraphLib {
    class UnionFind {
        int *parent;
        int *rank;

    public:
        explicit UnionFind(int n);

        ~UnionFind();

        int Find(int node) const;

        int CompressFind(int node);

        void Unite(int node_x, int node_y);

        void RankUnite(int node_x, int node_y);
    };
}

#endif
