#ifndef VECTOR2F_H
#define VECTOR2F_H

#include "Vector2.h"
#include <cmath>

namespace math{

class Vector2f : public Vector2<float> {
    public:

        Vector2f() 
        : Vector2() {};

        Vector2f(const float px, const float py) 
        : Vector2(px, py) {};

        float magnitude() const {
            return sqrt(x * x + y * y);
        }

        float magnitudeSquared() const {
            return x * x + y * y;
        }

        void normalize() {
            int mag = magnitude();

            x /= mag;
            y /= mag; 
        }

        Vector2f operator+(const Vector2f &vec) const {
            return Vector2f(x + vec.x, y + vec.y);
        }

        Vector2f operator-(const Vector2f &vec) const {
            return Vector2f(x - vec.x, y - vec.y);
        }

        Vector2f operator-( ) const {
            return Vector2f(-x, -y);
        }

        Vector2f operator*(const float num) const {
            return Vector2f(x * num, y * num);
        }

        Vector2f operator/(const float num) const {
            return Vector2f(x / num, y / num);
        }
};

inline float dot(const Vector2f &vec1, const Vector2f &vec2){
    return (vec1.x * vec2.x + vec1.y * vec2.y); 
}

}

#endif // VECTOR2F_H