#ifndef UNIONFIND_H
#define UNIONFIND_H

namespace GraphLib {
    class UnionFind {
        int *parent;
        int *rank;

    public:
        explicit UnionFind(int n);

        ~UnionFind();

        int SimpleFind(int x);

        int CompressFind(int x);

        void SimpleUnite(int x, int y);

        void RankUnite(int x, int y);
    };
}

#endif
