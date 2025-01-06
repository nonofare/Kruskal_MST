#ifndef GRAPH_H
#define GRAPH_H
#include "Point.h"
#include "UnionFind.h"
#include <string>
#include <vector>
#include <fstream>
#include <Python.h>
#include <unordered_map>

namespace GraphLib {
    class Graph {
        std::vector<Point *> points;
        std::vector<Pair> edges;

    public:
        Graph();

        ~Graph();

        void AddPoint(double x, double y);

        void MakePair(Point *first, Point *second, double weight);

        void Clear();

        void LoadFromFile(const std::string &file_path);

        std::vector<Pair> GetMST() const;

        std::string ToString(size_t points_limit = 0, size_t connections_limit = 0) const;

        std::string MSTToString(const std::vector<Pair> &mst, size_t limit = 0) const;

        void GraphDrawer() const;

        void MSTDrawer(const std::vector<Pair> &mst) const;
    };
}

#endif
