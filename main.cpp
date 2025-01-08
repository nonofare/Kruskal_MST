#include <iostream>
#include "Graph.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    auto graph = new GraphLib::Graph();
    try {
        graph->LoadFromFile(argv[1]);
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        delete graph;
        return 1;
    }

    const std::vector<GraphLib::Pair> mst = graph->GetMST();

    std::cout << graph->ToString(10, 10);
    graph->DrawWithMST(mst);

    delete graph;
    return 0;
}
