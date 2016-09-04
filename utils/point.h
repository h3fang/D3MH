#ifndef ENIGMA_POINT_H
#define ENIGMA_POINT_H

#include <cmath>

class Point {
public:
    float x;
    float y;

public:
    Point() : x(0), y(0) {}
    Point(float x_, float y_) : x(x_), y(y_) {}

    Point& operator+=(const Point& p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }

    Point& operator-=(const Point& p) {
        this->x -= p.x;
        this->y -= p.y;
        return *this;
    }

    Point operator+(const Point& p) {
        return Point(this->x+p.x, this->y+p.y);
    }

    Point operator-(const Point& p) {
        return Point(this->x-p.x, this->y-p.y);
    }
};

template<class T>
Point operator*(T num, const Point& p) {
    return Point(num*p.x, num*p.y);
}

template<class T>
Point operator*(const Point& p, T num) {
    return Point(num*p.x, num*p.y);
}

template<class T>
inline T distance(T x1, T y1, T x2, T y2) {
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

inline float distance(const Point& p1, const Point& p2) {
    return std::sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

#endif // ENIGMA_POINT_H
