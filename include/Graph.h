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
        std::vector<Pair *> edges;

    protected:
        void load_from_file(const std::string &file_path);

        void add_point(double x, double y, size_t index);

        void make_pair(Point *first, Point *second, double weight);

        std::vector<Pair *> get_sorted_edges() const;

    public:
        explicit Graph(const std::string &file_path);

        ~Graph();

        std::vector<Pair *> get_MST(bool rank_unite, bool path_compression,
                                    double &sort_time, double &loop_time,
                                    int &find_calls) const;

        std::string to_str(bool with_mst = false, size_t points_limit = 0,
                           size_t connections_limit = 0, size_t mst_limit = 0) const;

        void draw(bool with_mst = false) const;
    };
}

#endif
