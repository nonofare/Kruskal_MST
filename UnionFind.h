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

        int GetFindCalls() const;

        int Find(int node);

        int CompressFind(int node);

        void Unite(int root_x, int root_y);

        void RankUnite(int root_x, int root_y);
    };
}

#endif
