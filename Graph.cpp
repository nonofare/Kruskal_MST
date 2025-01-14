#include "Graph.h"

namespace GraphLib {
    Graph::Graph(const std::string &file_path) {
        try {
            LoadFromFile(file_path);
        } catch (const std::exception &ex) {
            throw ex;
        }
    }

    Graph::~Graph() {
        points.Erase();
        edges.Erase();
    }

    void Graph::LoadFromFile(const std::string &file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file from: " + file_path);
        }

        size_t points_size{}, edges_size{};

        file >> points_size;
        for (size_t i = 0; i < points_size; i++) {
            double x, y;
            file >> x >> y;
            AddPoint(x, y, i);
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

    void Graph::AddPoint(const double x, const double y, const size_t index) {
        points.Push(new Point(x, y, index));
    }

    void Graph::MakePair(Point *first, Point *second, const double weight) {
        edges.Push(new Pair(first, second, weight));
    }

    DA::DynArr<Pair *> Graph::GetSortedEdges() const {
        const size_t n = edges.Size();
        DA::DynArr<Pair *> sorted_edges;
        auto *buckets = new DA::DynArr<Pair *>[n];

        for (size_t i = 0; i < n; i++) {
            auto bucket_index = static_cast<size_t>(edges[i]->weight * static_cast<double>(n) / 1);
            buckets[bucket_index].Push(edges[i]);
        }

        size_t index = 0;
        for (size_t i = 0; i < n; i++) {
            if (!buckets[i].Empty()) {
                for (size_t j = 1; j < buckets[i].Size(); j++) {
                    Pair *key = buckets[i][j];
                    size_t k = j - 1;

                    while (k < buckets[i].Size() && buckets[i][k]->weight > key->weight) {
                        buckets[i][k + 1] = buckets[i][k];
                        k--;
                    }

                    buckets[i][k + 1] = key;
                }

                for (size_t j = 0; j < buckets[i].Size(); j++) {
                    sorted_edges.Push(buckets[i][j]);
                }
            }
        }

        delete[] buckets;
        return sorted_edges;
    }

    DA::DynArr<Pair *> Graph::GetMST(int &edges_size, double &weight, double &sort_time, double &loop_time,
                                     int &find_calls) const {
        auto start_sort_time = std::chrono::high_resolution_clock::now();
        DA::DynArr<Pair *> sorted_edges = GetSortedEdges();
        auto end_sort_time = std::chrono::high_resolution_clock::now();

        UnionFind uf(static_cast<int>(points.Size()));
        DA::DynArr<Pair *> mst;

        auto start_loop_time = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < sorted_edges.Size(); i++) {
            size_t index1 = sorted_edges[i]->first->index;
            size_t index2 = sorted_edges[i]->second->index;

            if (uf.CompressFind(static_cast<int>(index1)) != uf.CompressFind(static_cast<int>(index2))) {
                mst.Push(sorted_edges[i]);
                weight += sorted_edges[i]->weight;
                uf.RankUnite(static_cast<int>(index1), static_cast<int>(index2));
            }
        }
        auto end_loop_time = std::chrono::high_resolution_clock::now();

        edges_size = static_cast<int>(mst.Size());
        std::chrono::duration<double> mst_sort_time = end_sort_time - start_sort_time;
        sort_time = mst_sort_time.count();
        std::chrono::duration<double> mst_loop_time = end_loop_time - start_loop_time;
        loop_time = mst_loop_time.count();
        find_calls = static_cast<int>(sorted_edges.Size()) * 2;

        return mst;
    }

    std::string Graph::ToString(bool with_mst, size_t points_limit, size_t edges_limit, size_t mst_limit) const {
        if (points_limit <= 0 || points_limit > points.Size()) {
            points_limit = points.Size();
        }

        std::string text = ">>> Graph <<<\n";
        text += "Points: " + std::to_string(points.Size()) + "\n";
        for (size_t i = 0; i < points_limit; i++) {
            text += std::to_string(points[i]->index);
            text += ": (";
            text += std::to_string(points[i]->x);
            text += ", ";
            text += std::to_string(points[i]->y);
            text += ")\n";
        }

        if (points_limit < points.Size()) {
            text += "[...]\n";
        }

        if (edges_limit <= 0 || edges_limit > edges.Size()) {
            edges_limit = edges.Size();
        }

        text += "Edges: " + std::to_string(edges.Size()) + "\n";
        for (size_t i = 0; i < edges_limit; i++) {
            text += "( ";
            text += std::to_string(edges[i]->first->index);
            text += " )->( ";
            text += std::to_string(edges[i]->second->index);
            text += " ) weight: ";
            text += std::to_string(edges[i]->weight);
            text += "\n";
        }

        if (edges_limit < edges.Size()) {
            text += "[...]\n";
        }

        if (with_mst) {
            int mst_edges_size{}, mst_find_calls{};
            double mst_weight{}, mst_sort_time{}, mst_loop_time{};
            const DA::DynArr<Pair *> mst = this->GetMST(mst_edges_size, mst_weight, mst_sort_time, mst_loop_time,
                                                        mst_find_calls);

            if (mst_limit <= 0 || mst_limit > mst.Size()) {
                mst_limit = mst.Size();
            }

            text += "MST: \n";
            text += "edges: " + std::to_string(mst_edges_size) + "\n";
            text += "weight: " + std::to_string(mst_weight) + "\n";
            text += "sort time: " + std::to_string(mst_sort_time) + "s\n";
            text += "loop time: " + std::to_string(mst_loop_time) + "s\n";
            text += "find calls: " + std::to_string(mst_find_calls) + "\n";
            for (size_t i = 0; i < mst_limit; i++) {
                text += "( ";
                text += std::to_string(mst[i]->first->index);
                text += " )->( ";
                text += std::to_string(mst[i]->second->index);
                text += " ) weight: ";
                text += std::to_string(mst[i]->weight);
                text += "\n";
            }

            if (mst_limit < mst.Size()) {
                text += "[...]\n";
            }
        }

        return text;
    }
}
