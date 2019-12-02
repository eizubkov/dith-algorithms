#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

const double kEPS = 1e-10;

class Vector2D {
public:
    double x, y;

    Vector2D() : x(0.), y(0.) {}

    Vector2D(double x, double y);

    Vector2D operator*(double c);

    Vector2D operator+(Vector2D v1);

    Vector2D operator-(Vector2D v1);
};

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

// Add 2 Vectors
Vector2D Vector2D::operator+(Vector2D vec2d) {
    return {x + vec2d.x, y + vec2d.y};
}

// Subtract 2 vectors
Vector2D Vector2D::operator-(Vector2D vec2d) {
    return {x - vec2d.x, y - vec2d.y};
}

// Cross product of 2 vectors
Vector2D Vector2D::operator*(double c) {
    return {x * c, y * c};
}

bool IsAngleLess(const Vector2D &point1, const Vector2D &point2) {
    if ((point1.x > 0 && point2.x < 0) || (point1.x < 0 && point2.x > 0)) {
        return point1.x > 0 && point2.x < 0;
    }
    if (std::abs(point1.x) < kEPS) {
        return (point1.y > 0) ? (point2.x < 0) : false;
    }
    const double tg1 = point1.y / point1.x;
    const double tg2 = point2.y / point2.x;
    if (std::abs(point2.x) < kEPS) {
        return (point2.y > 0) ? (point1.x > 0) : (std::abs(tg1 - tg2) > kEPS);
    }
    return (std::abs(tg1 - tg2) >= kEPS) ? (tg1 < tg2) : false;
}

std::vector<Vector2D> MinkowskiSum(std::vector<Vector2D> polygon1,
                                   std::vector<Vector2D> polygon2) {
    const int polygon1_size = polygon1.size();
    const int polygon2_size = polygon2.size();

    polygon1.push_back(polygon1[0]);
    polygon2.push_back(polygon2[0]);

    int i = 0, j = 0;
    std::vector<Vector2D> result;
    while (i < polygon1_size && j < polygon2_size) {
        result.push_back(polygon1[i] + polygon2[j]);
        if (IsAngleLess(polygon2[j + 1] - polygon2[j],
                        polygon1[i + 1] - polygon1[i])) {
            ++j;
        } else if (IsAngleLess(polygon1[i + 1] - polygon1[i],
                               polygon2[j + 1] - polygon2[j])) {
            ++i;
        } else {
            ++i;
            ++j;
        }
    }
    return result;
}

bool IsNegative(const Vector2D &point1, const Vector2D &point2) {
    Vector2D vec1(point2.x - point1.x, point2.y - point1.y);
    Vector2D vec2(-point1.x, -point1.y);
    return (vec1.x * vec2.y - vec1.y * vec2.x) < 0;
}

bool IsCrossingVectors(std::vector<Vector2D> minkowski_sum) {
    minkowski_sum.push_back(minkowski_sum[0]);
    for (int i = 1; i < minkowski_sum.size(); ++i) {
        if ((IsNegative(minkowski_sum[i - 1], minkowski_sum[i]))) {
            return false;
        }
    }
    return true;
}

std::vector<Vector2D> CreatePolygon() {
    int n;
    std::cin >> n;
    std::vector<Vector2D> polygon(n);
    for (auto &point: polygon) {
        std::cin >> point.x >> point.y;
    }
    return polygon;
}

std::vector<Vector2D> Sorted(const std::vector<Vector2D> &polygon) {
    const int polygon_size = polygon.size();
    int min = 0;
    for (int i = 1; i < polygon_size; ++i) {
        if (polygon[i].x < polygon[min].x ||
            (std::abs(polygon[i].x - polygon[min].x) < kEPS
             && polygon[i].y < polygon[min].y)) {
            min = i;
        }
    }
    std::vector<Vector2D> result(polygon_size);
    for (int i = polygon_size - 1; i >= 0; --i) {
        result[i] = polygon[(polygon_size - i - 1 + min + 1) % polygon_size];
    }
    return result;
}

int main() {
    auto polygon1 = CreatePolygon();
    auto polygon2 = CreatePolygon();

    for (auto &point: polygon2) {
        point.x *= -1;
        point.y *= -1;
    }
    std::cout << (IsCrossingVectors(std::move(MinkowskiSum(
            std::move(Sorted(polygon1)),
            std::move(Sorted(polygon2)
            )))) ? "YES" : "NO") << std::endl;
    return 0;
}
