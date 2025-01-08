#include "Graph.h"

namespace GraphLib {
    void Graph::Draw() const {
        Py_Initialize();

        PyObject *x_list = PyList_New(static_cast<int>(points.size()));
        PyObject *y_list = PyList_New(static_cast<int>(points.size()));
        for (size_t i = 0; i < points.size(); i++) {
            PyList_SetItem(x_list, static_cast<long>(i), PyFloat_FromDouble(points[i]->x));
            PyList_SetItem(y_list, static_cast<long>(i), PyFloat_FromDouble(points[i]->y));
        }

        PyObject *start_x_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *start_y_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *end_x_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *end_y_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *weights_list = PyList_New(static_cast<int>(edges.size()));
        for (size_t i = 0; i < edges.size(); i++) {
            PyList_SetItem(start_x_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].first->x));
            PyList_SetItem(start_y_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].first->y));
            PyList_SetItem(end_x_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].second->x));
            PyList_SetItem(end_y_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].second->y));
            PyList_SetItem(weights_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].weight));
        }

        PyObject *main_module = PyImport_AddModule("__main__");
        PyObject *global_dict = PyModule_GetDict(main_module);
        PyDict_SetItemString(global_dict, "x_cords", x_list);
        PyDict_SetItemString(global_dict, "y_cords", y_list);
        PyDict_SetItemString(global_dict, "start_x", start_x_list);
        PyDict_SetItemString(global_dict, "start_y", start_y_list);
        PyDict_SetItemString(global_dict, "end_x", end_x_list);
        PyDict_SetItemString(global_dict, "end_y", end_y_list);
        PyDict_SetItemString(global_dict, "weights", weights_list);

        const char *script = R"(
import matplotlib.pyplot as plt

plt.scatter(x_cords, y_cords, label='Points', color='blue', marker='o')

#for x, y in zip(x_cords, y_cords):
    #plt.text(x, y, f'({x:.2f}, {y:.2f})', fontsize=8, ha='right', color='black')

for x1, y1, x2, y2, weight in zip(start_x, start_y, end_x, end_y, weights):
    plt.plot([x1, x2], [y1, y2], color='lightgray', linewidth=0.5)
    mid_x, mid_y = (x1 + x2) / 2, (y1 + y2) / 2
    #plt.text(mid_x, mid_y, f'{weight:.2f}', fontsize=8, ha='center', color='lightgray')

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

        Py_Finalize();
    }

    void Graph::DrawWithMST(const std::vector<Pair> &mst) const {
        Py_Initialize();

        PyObject *x_list = PyList_New(static_cast<int>(points.size()));
        PyObject *y_list = PyList_New(static_cast<int>(points.size()));
        for (size_t i = 0; i < points.size(); i++) {
            PyList_SetItem(x_list, static_cast<long>(i), PyFloat_FromDouble(points[i]->x));
            PyList_SetItem(y_list, static_cast<long>(i), PyFloat_FromDouble(points[i]->y));
        }

        PyObject *start_x_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *start_y_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *end_x_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *end_y_list = PyList_New(static_cast<int>(edges.size()));
        PyObject *weights_list = PyList_New(static_cast<int>(edges.size()));
        for (size_t i = 0; i < edges.size(); i++) {
            PyList_SetItem(start_x_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].first->x));
            PyList_SetItem(start_y_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].first->y));
            PyList_SetItem(end_x_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].second->x));
            PyList_SetItem(end_y_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].second->y));
            PyList_SetItem(weights_list, static_cast<long>(i), PyFloat_FromDouble(edges[i].weight));
        }

        PyObject *mst_start_x_list = PyList_New(static_cast<int>(mst.size()));
        PyObject *mst_start_y_list = PyList_New(static_cast<int>(mst.size()));
        PyObject *mst_end_x_list = PyList_New(static_cast<int>(mst.size()));
        PyObject *mst_end_y_list = PyList_New(static_cast<int>(mst.size()));
        for (size_t i = 0; i < mst.size(); i++) {
            PyList_SetItem(mst_start_x_list, static_cast<long>(i), PyFloat_FromDouble(mst[i].first->x));
            PyList_SetItem(mst_start_y_list, static_cast<long>(i), PyFloat_FromDouble(mst[i].first->y));
            PyList_SetItem(mst_end_x_list, static_cast<long>(i), PyFloat_FromDouble(mst[i].second->x));
            PyList_SetItem(mst_end_y_list, static_cast<long>(i), PyFloat_FromDouble(mst[i].second->y));
        }

        PyObject *main_module = PyImport_AddModule("__main__");
        PyObject *global_dict = PyModule_GetDict(main_module);
        PyDict_SetItemString(global_dict, "x_cords", x_list);
        PyDict_SetItemString(global_dict, "y_cords", y_list);
        PyDict_SetItemString(global_dict, "start_x", start_x_list);
        PyDict_SetItemString(global_dict, "start_y", start_y_list);
        PyDict_SetItemString(global_dict, "end_x", end_x_list);
        PyDict_SetItemString(global_dict, "end_y", end_y_list);
        PyDict_SetItemString(global_dict, "weights", weights_list);
        PyDict_SetItemString(global_dict, "mst_start_x", mst_start_x_list);
        PyDict_SetItemString(global_dict, "mst_start_y", mst_start_y_list);
        PyDict_SetItemString(global_dict, "mst_end_x", mst_end_x_list);
        PyDict_SetItemString(global_dict, "mst_end_y", mst_end_y_list);

        const char *script = R"(
import matplotlib.pyplot as plt

plt.scatter(x_cords, y_cords, label='Points', color='blue', marker='o')

#for x, y in zip(x_cords, y_cords):
    #plt.text(x, y, f'({x:.2f}, {y:.2f})', fontsize=8, ha='right', color='black')

for x1, y1, x2, y2, weight in zip(start_x, start_y, end_x, end_y, weights):
    plt.plot([x1, x2], [y1, y2], color='lightgray', linewidth=0.5)
    mid_x, mid_y = (x1 + x2) / 2, (y1 + y2) / 2
    #plt.text(mid_x, mid_y, f'{weight:.2f}', fontsize=8, ha='center', color='lightgray')

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
