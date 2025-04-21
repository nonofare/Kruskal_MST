#include "Graph.h"

namespace GraphLib {
    Graph::Graph(const std::string &file_path) {
        try {
            load_from_file(file_path);
        } catch (const std::exception &ex) {
            throw ex;
        }
    }

    Graph::~Graph() {
        points.clear();
        edges.clear();
    }

    void Graph::load_from_file(const std::string &file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file from: " + file_path);
        }

        size_t points_size{}, edges_size{};

        file >> points_size;
        for (size_t i = 0; i < points_size; i++) {
            double x, y;
            file >> x >> y;
            add_point(x, y, i);
        }

        file >> edges_size;
        for (size_t i = 0; i < edges_size; i++) {
            int index1, index2;
            double weight;
            file >> index1 >> index2 >> weight;
            make_pair(points[index1], points[index2], weight);
        }

        file.close();
    }

    void Graph::add_point(const double x, const double y, const size_t index) {
        points.push_back(new Point(x, y, index));
    }

    void Graph::make_pair(Point *first, Point *second, const double weight) {
        edges.push_back(new Pair(first, second, weight));
    }

    std::vector<Pair *> Graph::get_sorted_edges() const {
        const size_t n = edges.size();
        std::vector<Pair *> sorted_edges;
        auto *buckets = new std::vector<Pair *>[n];

        for (size_t i = 0; i < n; i++) {
            auto bucket_index = static_cast<size_t>(edges[i]->weight * static_cast<double>(n) / 1);
            buckets[bucket_index].push_back(edges[i]);
        }

        for (size_t i = 0; i < n; i++) {
            if (!buckets[i].empty()) {
                for (size_t j = 1; j < buckets[i].size(); j++) {
                    Pair *key = buckets[i][j];
                    size_t k = j - 1;

                    while (k < buckets[i].size() && buckets[i][k]->weight > key->weight) {
                        buckets[i][k + 1] = buckets[i][k];
                        k--;
                    }

                    buckets[i][k + 1] = key;
                }

                for (size_t j = 0; j < buckets[i].size(); j++) {
                    sorted_edges.push_back(buckets[i][j]);
                }
            }
        }

        delete[] buckets;
        return sorted_edges;
    }

    std::vector<Pair *> Graph::get_MST(const bool rank_unite, const bool path_compression,
                                       double &sort_time, double &loop_time,
                                       int &find_calls) const {
        UnionFind uf(static_cast<int>(points.size()));
        std::vector<Pair *> mst;

        const auto start_sort_time = std::chrono::high_resolution_clock::now();
        std::vector<Pair *> sorted_edges = get_sorted_edges();
        const auto end_sort_time = std::chrono::high_resolution_clock::now();

        const auto start_loop_time = std::chrono::high_resolution_clock::now();
        switch (rank_unite) {
            case true:
                switch (path_compression) {
                    case true:
                        for (size_t i = 0; i < sorted_edges.size(); i++) {
                            int index1 = uf.compress_find(static_cast<int>(sorted_edges[i]->first->index));
                            int index2 = uf.compress_find(static_cast<int>(sorted_edges[i]->second->index));

                            if (index1 != index2) {
                                mst.push_back(sorted_edges[i]);
                                uf.rank_unite(index1, index2);
                                if (mst.size() == points.size() - 1) {
                                    break;
                                }
                            }
                        }
                        break;

                    case false:
                        for (size_t i = 0; i < sorted_edges.size(); i++) {
                            int index1 = uf.find(static_cast<int>(sorted_edges[i]->first->index));
                            int index2 = uf.find(static_cast<int>(sorted_edges[i]->second->index));

                            if (index1 != index2) {
                                mst.push_back(sorted_edges[i]);
                                uf.rank_unite(index1, index2);
                                if (mst.size() == points.size() - 1) {
                                    break;
                                }
                            }
                        }
                        break;
                }
                break;

            case false:
                switch (path_compression) {
                    case true:
                        for (size_t i = 0; i < sorted_edges.size(); i++) {
                            int index1 = uf.compress_find(static_cast<int>(sorted_edges[i]->first->index));
                            int index2 = uf.compress_find(static_cast<int>(sorted_edges[i]->second->index));

                            if (index1 != index2) {
                                mst.push_back(sorted_edges[i]);
                                uf.unite(index1, index2);
                                if (mst.size() == points.size() - 1) {
                                    break;
                                }
                            }
                        }
                        break;

                    case false:
                        for (size_t i = 0; i < sorted_edges.size(); i++) {
                            int index1 = uf.find(static_cast<int>(sorted_edges[i]->first->index));
                            int index2 = uf.find(static_cast<int>(sorted_edges[i]->second->index));

                            if (index1 != index2) {
                                mst.push_back(sorted_edges[i]);
                                uf.unite(index1, index2);
                                if (mst.size() == points.size() - 1) {
                                    break;
                                }
                            }
                        }
                        break;
                }
                break;
        }
        const auto end_loop_time = std::chrono::high_resolution_clock::now();

        find_calls = uf.get_find_calls();
        const std::chrono::duration<double> mst_sort_time = end_sort_time - start_sort_time;
        sort_time = mst_sort_time.count();
        const std::chrono::duration<double> mst_loop_time = end_loop_time - start_loop_time;
        loop_time = mst_loop_time.count();

        return mst;
    }

    std::string Graph::to_str(bool with_mst, size_t points_limit, size_t connections_limit, size_t mst_limit) const {
        if (points_limit <= 0 || points_limit > points.size()) {
            points_limit = points.size();
        }

        std::string text = ">>> Graph <<<\n";
        text += "Points: " + std::to_string(points.size()) + "\n";
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

        if (points_limit < points.size()) {
            text += "  [...]\n";
        }
        text += "}\n";

        if (connections_limit <= 0 || connections_limit > edges.size()) {
            connections_limit = edges.size();
        }

        text += "Edges: " + std::to_string(edges.size()) + "\n";
        text += "{\n";
        for (size_t i = 0; i < connections_limit; i++) {
            text += "  ( ";
            text += std::to_string(edges[i]->first->index);
            text += " )->( ";
            text += std::to_string(edges[i]->second->index);
            text += " ) weight: ";
            text += std::to_string(edges[i]->weight);
            text += "\n";
        }

        if (connections_limit < edges.size()) {
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
            const std::vector<Pair *> mst1 = this->get_MST(true, true,
                                                           mst_sort_time, mst1_loop_time,
                                                           mst1_find_calls);
            const std::vector<Pair *> mst2 = this->get_MST(false, true,
                                                           mst_sort_time, mst2_loop_time,
                                                           mst2_find_calls);
            const std::vector<Pair *> mst3 = this->get_MST(true, false,
                                                           mst_sort_time, mst3_loop_time,
                                                           mst3_find_calls);
            const std::vector<Pair *> mst4 = this->get_MST(false, false,
                                                           mst_sort_time, mst4_loop_time,
                                                           mst4_find_calls);

            if (mst_limit <= 0 || mst_limit > mst1.size()) {
                mst_limit = mst1.size();
            }

            double weight{};
            for (size_t i = 0; i < mst1.size(); i++) {
                weight += mst1[i]->weight;
            }

            text += "MST: \n";
            text += "edges: " + std::to_string(mst1.size()) + "\n";
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

            if (mst_limit < mst1.size()) {
                text += "  [...]\n";
            }
            text += "}\n";
        }

        return text;
    }

    void Graph::draw(const bool with_mst) const {
        Py_Initialize();
        PyObject *main_module = PyImport_AddModule("__main__");
        PyObject *global_dict = PyModule_GetDict(main_module);

        PyObject *x_list = PyList_New(points.size());
        PyObject *y_list = PyList_New(points.size());
        PyObject *index_list = PyList_New(points.size());
        for (size_t i = 0; i < points.size(); i++) {
            PyList_SetItem(x_list, i, PyFloat_FromDouble(points[i]->x));
            PyList_SetItem(y_list, i, PyFloat_FromDouble(points[i]->y));
            PyList_SetItem(index_list, i, PyLong_FromUnsignedLong(points[i]->index));
        }
        PyDict_SetItemString(global_dict, "x_cords", x_list);
        PyDict_SetItemString(global_dict, "y_cords", y_list);
        PyDict_SetItemString(global_dict, "indexes", index_list);

        PyObject *start_x_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *start_y_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *end_x_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *end_y_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *weights_list = PyList_New(static_cast<int>(edges.size()));
        for (size_t i = 0; i < edges.size(); i++) {
            PyList_SetItem(start_x_list, i, PyFloat_FromDouble(edges[i]->first->x));
            PyList_SetItem(start_y_list, i, PyFloat_FromDouble(edges[i]->first->y));
            PyList_SetItem(end_x_list, i, PyFloat_FromDouble(edges[i]->second->x));
            PyList_SetItem(end_y_list, i, PyFloat_FromDouble(edges[i]->second->y));
            PyList_SetItem(weights_list, i, PyFloat_FromDouble(edges[i]->weight));
        }
        PyDict_SetItemString(global_dict, "start_x", start_x_list);
        PyDict_SetItemString(global_dict, "start_y", start_y_list);
        PyDict_SetItemString(global_dict, "end_x", end_x_list);
        PyDict_SetItemString(global_dict, "end_y", end_y_list);
        PyDict_SetItemString(global_dict, "weights", weights_list);

        PyObject *mst_start_x_list = nullptr;
        PyObject *mst_start_y_list = nullptr;
        PyObject *mst_end_x_list = nullptr;
        PyObject *mst_end_y_list = nullptr;
        if (with_mst) {
            int mst_find_calls{};
            double mst_sort_time{}, mst_loop_time{};
            const std::vector<Pair *> mst = this->get_MST(true, true,
                                                          mst_sort_time, mst_loop_time,
                                                          mst_find_calls);

            mst_start_x_list = PyList_New(static_cast<int>(mst.size()));
            mst_start_y_list = PyList_New(static_cast<int>(mst.size()));
            mst_end_x_list = PyList_New(static_cast<int>(mst.size()));
            mst_end_y_list = PyList_New(static_cast<int>(mst.size()));
            for (size_t i = 0; i < mst.size(); i++) {
                PyList_SetItem(mst_start_x_list, i, PyFloat_FromDouble(mst[i]->first->x));
                PyList_SetItem(mst_start_y_list, i, PyFloat_FromDouble(mst[i]->first->y));
                PyList_SetItem(mst_end_x_list, i, PyFloat_FromDouble(mst[i]->second->x));
                PyList_SetItem(mst_end_y_list, i, PyFloat_FromDouble(mst[i]->second->y));
            }
            PyDict_SetItemString(global_dict, "mst_start_x", mst_start_x_list);
            PyDict_SetItemString(global_dict, "mst_start_y", mst_start_y_list);
            PyDict_SetItemString(global_dict, "mst_end_x", mst_end_x_list);
            PyDict_SetItemString(global_dict, "mst_end_y", mst_end_y_list);
        }

        const auto script = R"(
import matplotlib.pyplot as plt

plt.figure(figsize=(15,15))
plt.scatter(x_cords, y_cords, label='Points', color='blue', marker='o')

for x, y, idx in zip(x_cords, y_cords, indexes):
    plt.text(x, y, str(idx), fontsize=8, ha='right', va='bottom', color='black')

for x1, y1, x2, y2, weight in zip(start_x, start_y, end_x, end_y, weights):
    plt.plot([x1, x2], [y1, y2], color='lightgray', linewidth=0.5)

if 'mst_start_x' in globals() and mst_start_x is not None:
    for x1, y1, x2, y2 in zip(mst_start_x, mst_start_y, mst_end_x, mst_end_y):
        plt.plot([x1, x2], [y1, y2], color='red', linewidth=1.5)

plt.xlabel('x')
plt.ylabel('y')
plt.title('Graph')
plt.legend()

plt.savefig('plots/test.png')
)";

        PyRun_SimpleString(script);

        Py_DECREF(x_list);
        Py_DECREF(y_list);
        Py_DECREF(start_x_list);
        Py_DECREF(start_y_list);
        Py_DECREF(end_x_list);
        Py_DECREF(end_y_list);
        Py_DECREF(weights_list);
        Py_DECREF(mst_start_x_list);
        Py_DECREF(mst_start_y_list);
        Py_DECREF(mst_end_x_list);
        Py_DECREF(mst_end_y_list);

        Py_Finalize();
    }
}
