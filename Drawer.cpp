#include "Graph.h"

namespace GraphLib {
    void Graph::Draw(const bool with_mst) const {
        Py_Initialize();
        PyObject *main_module = PyImport_AddModule("__main__");
        PyObject *global_dict = PyModule_GetDict(main_module);

        PyObject *x_list = PyList_New(static_cast<int>(points.Size()));
        PyObject *y_list = PyList_New(static_cast<int>(points.Size()));
        PyObject *index_list = PyList_New(static_cast<int>(points.Size()));
        for (size_t i = 0; i < points.Size(); i++) {
            PyList_SetItem(x_list, static_cast<long>(i), PyFloat_FromDouble(points[i]->x));
            PyList_SetItem(y_list, static_cast<long>(i), PyFloat_FromDouble(points[i]->y));
            PyList_SetItem(index_list, static_cast<long>(i), PyLong_FromUnsignedLong(points[i]->index));
        }
        PyDict_SetItemString(global_dict, "x_cords", x_list);
        PyDict_SetItemString(global_dict, "y_cords", y_list);
        PyDict_SetItemString(global_dict, "indexes", index_list);

        PyObject *start_x_list = PyList_New(static_cast<int>(edges.Size()));
        PyObject *start_y_list = PyList_New(static_cast<int>(edges.Size()));
        PyObject *end_x_list = PyList_New(static_cast<int>(edges.Size()));
        PyObject *end_y_list = PyList_New(static_cast<int>(edges.Size()));
        PyObject *weights_list = PyList_New(static_cast<int>(edges.Size()));
        for (size_t i = 0; i < edges.Size(); i++) {
            PyList_SetItem(start_x_list, static_cast<long>(i), PyFloat_FromDouble(edges[i]->first->x));
            PyList_SetItem(start_y_list, static_cast<long>(i), PyFloat_FromDouble(edges[i]->first->y));
            PyList_SetItem(end_x_list, static_cast<long>(i), PyFloat_FromDouble(edges[i]->second->x));
            PyList_SetItem(end_y_list, static_cast<long>(i), PyFloat_FromDouble(edges[i]->second->y));
            PyList_SetItem(weights_list, static_cast<long>(i), PyFloat_FromDouble(edges[i]->weight));
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
            const DA::DynArr<Pair *> mst = this->GetMST(true, true,
                                                        mst_sort_time, mst_loop_time,
                                                        mst_find_calls);

            mst_start_x_list = PyList_New(static_cast<int>(mst.Size()));
            mst_start_y_list = PyList_New(static_cast<int>(mst.Size()));
            mst_end_x_list = PyList_New(static_cast<int>(mst.Size()));
            mst_end_y_list = PyList_New(static_cast<int>(mst.Size()));
            for (size_t i = 0; i < mst.Size(); i++) {
                PyList_SetItem(mst_start_x_list, static_cast<long>(i), PyFloat_FromDouble(mst[i]->first->x));
                PyList_SetItem(mst_start_y_list, static_cast<long>(i), PyFloat_FromDouble(mst[i]->first->y));
                PyList_SetItem(mst_end_x_list, static_cast<long>(i), PyFloat_FromDouble(mst[i]->second->x));
                PyList_SetItem(mst_end_y_list, static_cast<long>(i), PyFloat_FromDouble(mst[i]->second->y));
            }
            PyDict_SetItemString(global_dict, "mst_start_x", mst_start_x_list);
            PyDict_SetItemString(global_dict, "mst_start_y", mst_start_y_list);
            PyDict_SetItemString(global_dict, "mst_end_x", mst_end_x_list);
            PyDict_SetItemString(global_dict, "mst_end_y", mst_end_y_list);
        }

        const auto script = R"(
import matplotlib.pyplot as plt

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

plt.show()
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
