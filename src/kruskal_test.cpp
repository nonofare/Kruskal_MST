#include <iostream>
#include "../include/Graph.h"

int main() {
    GraphLib::Graph *g1, *g2, *g3;
    try {
        g1 = new GraphLib::Graph("examples/g1.txt");
        g2 = new GraphLib::Graph("examples/g2.txt");
        g3 = new GraphLib::Graph("examples/g3.txt");
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << g1->to_str(true, 4, 4, 8) << std::endl;
    std::cout << g2->to_str(true, 4, 4, 8) << std::endl;
    std::cout << g3->to_str(true, 4, 4, 8) << std::endl;

    g1->draw(true);
    //g2->draw(true);
    //g3->draw(true);

    delete g1;
    delete g2;
    delete g3;
    return EXIT_SUCCESS;
}
