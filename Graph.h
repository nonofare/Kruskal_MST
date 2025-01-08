#ifndef GRAPH_H
#define GRAPH_H
#include "Point.h"
#include "UnionFind.h"
#include <string>
#include <vector>
#include <fstream>
#include <Python.h>
#include <unordered_map> // temp

namespace GraphLib {
    class Graph {
        std::vector<Point *> points;
        std::vector<Pair> edges;

    public:
        Graph();

        ~Graph();

        void LoadFromFile(const std::string &file_path);

        void AddPoint(double x, double y);

        void MakePair(Point *first, Point *second, double weight);

        std::vector<Pair> GetMST() const;

        void Clear();

        std::string ToString(size_t points_limit = 0, size_t connections_limit = 0) const;

        void Draw() const;

        void DrawWithMST(const std::vector<Pair> &mst) const;
    };
}

#endif
