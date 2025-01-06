#include <iostream>
#include "Graph.h"

int main() {
    auto graph = new GraphLib::Graph();
    try {
        graph->LoadFromFile("/Users/adamwojturski/Desktop/excercises/g2.txt");
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        delete graph;
        return 1;
    }

    std::vector<GraphLib::Pair> mst = graph->GetMST();

    std::cout << graph->ToString(4, 4);
    std::cout << graph->MSTToString(mst, 4);
    graph->MSTDrawer(mst);

    delete graph;
    return 0;
}
