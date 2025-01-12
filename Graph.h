#ifndef GRAPH_H
#define GRAPH_H
#include "Point.h"
#include "UnionFind.h"
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <Python.h>

namespace GraphLib {
    class Graph {
        std::vector<Point *> points;
        std::vector<Pair> edges;

    protected:
        void LoadFromFile(const std::string &file_path);

        void AddPoint(double x, double y, size_t index);

        void MakePair(Point *first, Point *second, double weight);

        std::vector<Pair> GetSortedEdges() const;

    public:
        explicit Graph(const std::string &file_path);

        ~Graph();

        std::vector<Pair> GetMST(int &edges_size, double &weight, double &sort_time, double &loop_time,
                                 int &find_calls) const;

        std::string ToString(bool with_mst = false, size_t points_limit = 0, size_t connections_limit = 0,
                             size_t mst_limit = 0) const;

        void Draw(bool with_mst = false) const;
    };
}

#endif
