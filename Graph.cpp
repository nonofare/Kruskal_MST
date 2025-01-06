#include "Graph.h"

namespace GraphLib {
    Graph::Graph() = default;

    Graph::~Graph() {
        Graph::Clear();
    }

    void Graph::AddPoint(const double x, const double y) {
        points.push_back(new Point(x, y));
    }

    void Graph::MakePair(Point *first, Point *second, const double weight) {
        edges.emplace_back(first, second, weight);
    }

    void Graph::Clear() {
        for (size_t i = 0; i < points.size(); i++) {
            delete points[i];
        }
        points.clear();
        edges.clear();
    }

    void Graph::LoadFromFile(const std::string &file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file from: " + file_path);
        }

        Clear();

        size_t points_size, edges_size;

        file >> points_size;
        for (size_t i = 0; i < points_size; i++) {
            double x, y;
            file >> x >> y;
            AddPoint(x, y);
        }

        file >> edges_size;
        for (size_t i = 0; i < edges_size; i++) {
            int index1, index2;
            double weight;
            file >> index1 >> index2 >> weight;
            MakePair(points[index1], points[index2], weight);
        }

        file.close();
    }

    std::vector<Pair> Graph::GetMST() const {
        std::vector<Pair> sorted_edges = edges;
        std::ranges::sort(sorted_edges, [](const Pair &c1, const Pair &c2) {
            return c1.weight < c2.weight;
        });

        UnionFind uf(static_cast<int>(points.size()));

        std::unordered_map<Point *, int> point_to_index;
        for (size_t i = 0; i < points.size(); ++i) {
            point_to_index[points[i]] = static_cast<int>(i);
        }

        std::vector<Pair> mst;
        for (const auto &edge: sorted_edges) {
            int index1 = point_to_index.at(edge.first);
            int index2 = point_to_index.at(edge.second);

            if (uf.CompressFind(index1) != uf.CompressFind(index2)) {
                mst.push_back(edge);

                uf.RankUnite(index1, index2);

                if (mst.size() == points.size() - 1) {
                    break;
                }
            }
        }

        return mst;
    }

    std::string Graph::ToString(size_t points_limit, size_t edges_limit) const {
        if (points_limit <= 0 || points_limit > points.size()) {
            points_limit = points.size();
        }

        std::string text = ">>> Graph <<<\n";
        text += "Points: " + std::to_string(points.size()) + "\n";
        for (size_t i = 0; i < points_limit; i++) {
            text += std::to_string(i + 1);
            text += ": (";
            text += std::to_string(points[i]->x);
            text += ", ";
            text += std::to_string(points[i]->y);
            text += ")\n";
        }

        if (points_limit < points.size()) {
            text += "[...]\n";
        }

        if (edges_limit <= 0 || edges_limit > edges.size()) {
            edges_limit = edges.size();
        }

        text += "Edges: " + std::to_string(edges.size()) + "\n";
        for (size_t i = 0; i < edges_limit; i++) {
            text += "(";
            text += std::to_string(edges[i].first->x);
            text += ", ";
            text += std::to_string(edges[i].first->y);
            text += ") -> (";
            text += std::to_string(edges[i].second->x);
            text += ", ";
            text += std::to_string(edges[i].second->y);
            text += ") weight: ";
            text += std::to_string(edges[i].weight);
            text += "\n";
        }

        if (edges_limit < edges.size()) {
            text += "[...]\n";
        }

        return text;
    }

    std::string Graph::MSTToString(const std::vector<Pair> &mst, size_t limit) const {
        if (limit <= 0 || limit > mst.size()) {
            limit = mst.size();
        }

        double weight = 0.0;
        std::string text = ">>> MST <<<\n";
        for (size_t i = 0; i < limit; i++) {
            weight += mst[i].weight;

            text += "(";
            text += std::to_string(mst[i].first->x);
            text += ", ";
            text += std::to_string(mst[i].first->y);
            text += ") -> ";
            text += "(";
            text += std::to_string(mst[i].second->x);
            text += ", ";
            text += std::to_string(mst[i].second->y);
            text += ") -> \n";
        }
        text += "Full path weight: " + std::to_string(weight) + "\n";

        if (limit < mst.size()) {
            text += "[...]\n";
        }

        return text;
    }
}
