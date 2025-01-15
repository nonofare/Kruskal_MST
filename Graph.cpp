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

    DA::DynArr<Pair *> Graph::GetMST(bool RankUnite, bool PathCompression,
                                     double &sort_time, double &loop_time,
                                     int &find_calls) const {
        UnionFind uf(static_cast<int>(points.Size()));
        DA::DynArr<Pair *> mst;

        const auto start_sort_time = std::chrono::high_resolution_clock::now();
        DA::DynArr<Pair *> sorted_edges = GetSortedEdges();
        const auto end_sort_time = std::chrono::high_resolution_clock::now();

        const auto start_loop_time = std::chrono::high_resolution_clock::now();
        switch (RankUnite) {
            case true:
                switch (PathCompression) {
                    case true:
                        for (size_t i = 0; i < sorted_edges.Size(); i++) {
                            int index1 = uf.CompressFind(static_cast<int>(sorted_edges[i]->first->index));
                            int index2 = uf.CompressFind(static_cast<int>(sorted_edges[i]->second->index));

                            if (index1 != index2) {
                                mst.Push(sorted_edges[i]);
                                uf.RankUnite(index1, index2);
                                if (mst.Size() == points.Size() - 1) {
                                    break;
                                }
                            }
                        }
                        break;

                    case false:
                        for (size_t i = 0; i < sorted_edges.Size(); i++) {
                            int index1 = uf.Find(static_cast<int>(sorted_edges[i]->first->index));
                            int index2 = uf.Find(static_cast<int>(sorted_edges[i]->second->index));

                            if (index1 != index2) {
                                mst.Push(sorted_edges[i]);
                                uf.RankUnite(index1, index2);
                                if (mst.Size() == points.Size() - 1) {
                                    break;
                                }
                            }
                        }
                        break;
                }
                break;

            case false:
                switch (PathCompression) {
                    case true:
                        for (size_t i = 0; i < sorted_edges.Size(); i++) {
                            int index1 = uf.CompressFind(static_cast<int>(sorted_edges[i]->first->index));
                            int index2 = uf.CompressFind(static_cast<int>(sorted_edges[i]->second->index));

                            if (index1 != index2) {
                                mst.Push(sorted_edges[i]);
                                uf.Unite(index1, index2);
                                if (mst.Size() == points.Size() - 1) {
                                    break;
                                }
                            }
                        }
                        break;

                    case false:
                        for (size_t i = 0; i < sorted_edges.Size(); i++) {
                            int index1 = uf.Find(static_cast<int>(sorted_edges[i]->first->index));
                            int index2 = uf.Find(static_cast<int>(sorted_edges[i]->second->index));

                            if (index1 != index2) {
                                mst.Push(sorted_edges[i]);
                                uf.Unite(index1, index2);
                                if (mst.Size() == points.Size() - 1) {
                                    break;
                                }
                            }
                        }
                        break;
                }
                break;
        }
        const auto end_loop_time = std::chrono::high_resolution_clock::now();

        find_calls = uf.GetFindCalls();
        std::chrono::duration<double> mst_sort_time = end_sort_time - start_sort_time;
        sort_time = mst_sort_time.count();
        std::chrono::duration<double> mst_loop_time = end_loop_time - start_loop_time;
        loop_time = mst_loop_time.count();

        return mst;
    }

    std::string Graph::ToString(bool with_mst, size_t points_limit, size_t edges_limit, size_t mst_limit) const {
        if (points_limit <= 0 || points_limit > points.Size()) {
            points_limit = points.Size();
        }

        std::string text = ">>> Graph <<<\n";
        text += "Points: " + std::to_string(points.Size()) + "\n";
        text += "{\n";
        for (size_t i = 0; i < points_limit; i++) {
            text += "  ";
            text += std::to_string(points[i]->index);
            text += ": (";
            text += std::to_string(points[i]->x);
            text += ", ";
            text += std::to_string(points[i]->y);
            text += ")\n";
        }

        if (points_limit < points.Size()) {
            text += "  [...]\n";
        }
        text += "}\n";

        if (edges_limit <= 0 || edges_limit > edges.Size()) {
            edges_limit = edges.Size();
        }

        text += "Edges: " + std::to_string(edges.Size()) + "\n";
        text += "{\n";
        for (size_t i = 0; i < edges_limit; i++) {
            text += "  ( ";
            text += std::to_string(edges[i]->first->index);
            text += " )->( ";
            text += std::to_string(edges[i]->second->index);
            text += " ) weight: ";
            text += std::to_string(edges[i]->weight);
            text += "\n";
        }

        if (edges_limit < edges.Size()) {
            text += "  [...]\n";
        }
        text += "}\n";

        if (with_mst) {
            int mst1_find_calls{},
                    mst2_find_calls{},
                    mst3_find_calls{},
                    mst4_find_calls{};
            double mst_sort_time{},
                    mst1_loop_time{},
                    mst2_loop_time{},
                    mst3_loop_time{},
                    mst4_loop_time{};
            const DA::DynArr<Pair *> mst1 = this->GetMST(true, true,
                                                         mst_sort_time, mst1_loop_time,
                                                         mst1_find_calls);
            const DA::DynArr<Pair *> mst2 = this->GetMST(false, true,
                                                         mst_sort_time, mst2_loop_time,
                                                         mst2_find_calls);
            const DA::DynArr<Pair *> mst3 = this->GetMST(true, false,
                                                         mst_sort_time, mst3_loop_time,
                                                         mst3_find_calls);
            const DA::DynArr<Pair *> mst4 = this->GetMST(false, false,
                                                         mst_sort_time, mst4_loop_time,
                                                         mst4_find_calls);

            if (mst_limit <= 0 || mst_limit > mst1.Size()) {
                mst_limit = mst1.Size();
            }

            double weight{};
            for (size_t i = 0; i < mst1.Size(); i++) {
                weight += mst1[i]->weight;
            }

            text += "MST: \n";
            text += "edges: " + std::to_string(mst1.Size()) + "\n";
            text += "weight: " + std::to_string(weight) + "\n";
            text += "sort time: " + std::to_string(mst_sort_time) + "s\n";
            text += "kruskal time:\n";
            text += "  path compression | rank unite: " + std::to_string(mst1_loop_time) + "s\n";
            text += "  path compression |    ----   : " + std::to_string(mst2_loop_time) + "s\n";
            text += "        ----       | rank unite: " + std::to_string(mst3_loop_time) + "s\n";
            text += "        ----       |    ----   : " + std::to_string(mst4_loop_time) + "s\n";
            text += "find calls:\n";
            text += "  path compression | rank unite: " + std::to_string(mst1_find_calls) + "\n";
            text += "  path compression |    ----   : " + std::to_string(mst2_find_calls) + "\n";
            text += "        ----       | rank unite: " + std::to_string(mst3_find_calls) + "\n";
            text += "        ----       |    ----   : " + std::to_string(mst4_find_calls) + "\n";

            text += "{\n";
            for (size_t i = 0; i < mst_limit; i++) {
                text += "  ( ";
                text += std::to_string(mst1[i]->first->index);
                text += " )->( ";
                text += std::to_string(mst1[i]->second->index);
                text += " ) weight: ";
                text += std::to_string(mst1[i]->weight);
                text += "\n";
            }

            if (mst_limit < mst1.Size()) {
                text += "  [...]\n";
            }
            text += "}\n";
        }

        return text;
    }
}
