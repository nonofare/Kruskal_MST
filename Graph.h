#ifndef GRAPH_H
#define GRAPH_H
#include "DA.h"
#include "Point.h"
#include "UnionFind.h"
#include <string>
#include <fstream>
#include <chrono>
#include <Python.h>

namespace GraphLib {
    class Graph {
        DA::DynArr<Point *> points;
        DA::DynArr<Pair *> edges;

    protected:
        void LoadFromFile(const std::string &file_path);

        void AddPoint(double x, double y, size_t index);

        void MakePair(Point *first, Point *second, double weight);

        DA::DynArr<Pair *> GetSortedEdges() const;

    public:
        explicit Graph(const std::string &file_path);

        ~Graph();

        DA::DynArr<Pair *> GetMST(bool RankUnite, bool PathCompression,
                                  double &sort_time, double &loop_time,
                                  int &find_calls) const;

        std::string ToString(bool with_mst = false, size_t points_limit = 0,
                             size_t connections_limit = 0, size_t mst_limit = 0) const;

        void Draw(bool with_mst = false) const;
    };
}

#endif
