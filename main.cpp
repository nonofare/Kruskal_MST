#include <iostream>
#include "Graph.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <file1> <file2> <file3>" << std::endl;
        return EXIT_FAILURE;
    }

    GraphLib::Graph *g1, *g2, *g3;
    try {
        g1 = new GraphLib::Graph(argv[1]);
        g2 = new GraphLib::Graph(argv[2]);
        g3 = new GraphLib::Graph(argv[3]);
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << g1->ToString(true, 10, 10, 10);
    std::cout << g2->ToString(true, 10, 10, 10);
    std::cout << g3->ToString(true, 10, 10, 10);

    //g1->Draw(true);
    g2->Draw(true);
    //g3->Draw(true);

    delete g1;
    delete g2;
    delete g3;
    return EXIT_SUCCESS;
}
