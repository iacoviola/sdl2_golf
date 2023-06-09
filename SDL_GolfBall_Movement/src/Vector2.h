#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>

namespace math {
template <typename T>
class Vector2
{
    public:

        T x, y;

        Vector2() 
        : x(0), y(0) {};

        Vector2(const T px, const T py) 
        : x(px), y(py) {};

        Vector2& operator+=(const Vector2 &vec){
            x += vec.x;
            y += vec.y;

            return *this;
        }

        Vector2& operator-=(const Vector2 &vec){
            x -= vec.x;
            y -= vec.y;

            return *this;
        }

        Vector2& operator*=(const T num){
            x *= num;
            y *= num;

            return *this;
        }

        bool operator==(const Vector2 &vec){
            return (x == vec.x && y == vec.y);
        }

        Vector2<T> operator+(const Vector2<T> &vec) const {
            return Vector2<T>(x + vec.x, y + vec.y);
        }
        
        Vector2<T> operator-(const Vector2<T> &vec) const {
            return Vector2<T>(x - vec.x, y - vec.y);
        }

        Vector2<T> operator-( ) const {
            return Vector2<T>(-x, -y);
        }

        Vector2<T> operator*(const T num) const {
            return Vector2<T>(x * num, y * num);
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2 &vec){
            os << "(" << vec.x << ", " << vec.y << ")";
            return os;
        }

};

}

#endif // VECTOR2_H