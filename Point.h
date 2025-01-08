#ifndef POINT_H
#define POINT_H

namespace GraphLib {
    struct Point {
        double x, y;

        Point(const double x, const double y) : x(x), y(y) {
        }
    };

    struct Pair {
        Point *first, *second;
        double weight;

        Pair(Point *first, Point *second, const double weight = 1) : first(first), second(second), weight(weight) {
        }
    };
}

#endif
