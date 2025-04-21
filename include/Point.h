#ifndef POINT_H
#define POINT_H

namespace GraphLib {
    struct Point {
        double x, y;
        unsigned long long index;

        Point(const double x, const double y, const unsigned long long index = 0) : x(x), y(y), index(index) {
        }
    };

    struct Pair {
        Point *first, *second;
        double weight;

        Pair(Point *first, Point *second, const double weight = 1.0) : first(first), second(second), weight(weight) {
        }
    };
}

#endif
