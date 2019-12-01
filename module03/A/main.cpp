#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

const double kEPS = 1e-10;

class Vector3D {
public:
    double x, y, z;
    // 3D Coordinates of the Vector

    Vector3D();

    Vector3D(double x, double y, double z);

    Vector3D operator+(Vector3D vec3d);

    Vector3D operator-(Vector3D vec3d);

    double operator^(Vector3D vec3d);

    Vector3D operator*(double c);

    double magnitude() { return sqrt((x * x) + (y * y) + (z * z)); }
};

Vector3D::Vector3D() : x(0.), y(0.), z(0.) {}

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

// Add 2 Vectors
Vector3D Vector3D::operator+(Vector3D vec3d) {
    return {x + vec3d.x, y + vec3d.y, z + vec3d.z};
}

// Subtract 2 vectors
Vector3D Vector3D::operator-(Vector3D vec3d) {
    return {x - vec3d.x, y - vec3d.y, z - vec3d.z};
}

// Cross product of 2 vectors
Vector3D Vector3D::operator*(double c) {
    return {x * c, y * c, z * c};
}

// Dot product of 2 vectors
double Vector3D::operator^(Vector3D vec3d) {
    return (x * vec3d.x) + (y * vec3d.y) + (z * vec3d.z);
}


double DistanceBetweenPoints(const Vector3D &vec1, const Vector3D &vec2) {
    return sqrt(pow(vec2.x - vec1.x, 2)
                + pow(vec2.y - vec1.y, 2) + pow(vec2.z - vec1.z, 2));
}

Vector3D Middle(const Vector3D &point1, const Vector3D &point2) {
    return {((point1.x + point2.x) / 2),
            ((point1.y + point2.y) / 2),
            ((point1.z + point2.z) / 2)};
}

double DistanceBetweenSegmentAndPoint(const Vector3D &point1,
                                      const Vector3D &point2,
                                      const Vector3D &point3) {
    Vector3D right = point1;
    Vector3D left = point2;
    while (DistanceBetweenPoints(right, left) > kEPS) {
        const auto mid1 = Middle(right, left);
        const auto mid2 = Middle(mid1, left);
        const auto mid3 = Middle(mid1, right);
        if (DistanceBetweenPoints(mid2, point3) -
            DistanceBetweenPoints(mid3, point3) < kEPS) {
            right = mid3;
        } else {
            left = mid2;
        }
    }
    return DistanceBetweenPoints(left, point3);
}

double DistanceBetweenSegments(const Vector3D &point1, const Vector3D &point2,
                               const Vector3D &point3, const Vector3D &point4) {
    Vector3D right = point1;
    Vector3D left = point2;
    while (DistanceBetweenPoints(right, left) > kEPS) {
        const auto mid1 = Middle(right, left);
        const auto mid2 = Middle(mid1, left);
        const auto mid3 = Middle(mid1, right);
        if (DistanceBetweenSegmentAndPoint(point3, point4, mid2) -
            DistanceBetweenSegmentAndPoint(point3, point4, mid3) < kEPS) {
            right = mid3;
        } else {
            left = mid2;
        }
    }
    return DistanceBetweenSegmentAndPoint(point3, point4, left);
}

int main() {
    std::vector<Vector3D> points(4);
    for (auto &vec3d: points) {
        std::cin >> vec3d.x >> vec3d.y >> vec3d.z;
    }
    std::cout << std::setprecision(20)
              << DistanceBetweenSegments(points[0], points[1], points[2], points[3])
              << std::endl;
    return 0;
}
