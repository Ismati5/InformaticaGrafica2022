#include "direction.hpp"


    //direction + direction
    Direction Direction::operator+(const Direction &d) const{
        return Direction(c[0] + d.c[0],
            c[1] + d.c[1], c[2] + d.c[2]);
    }
    //direction + point
    Direction Direction::operator+(const Point &p) const{
        return Direction(c[0] + p.c[0],
            c[1] + p.c[1], c[2] + p.c[2]);
    }
    //direction - direction
    Direction Direction::operator-(const Direction &d) const{
        return Direction(c[0] - d.c[0],
            c[1] - d.c[1], c[2] - d.c[2]);
    }
    //direction + point
    Direction Direction::operator-(const Point &p) const{
        return Direction(c[0] - p.c[0],
            c[1] - p.c[1], c[2] - p.c[2]);
    }
    //direction * scalar
    Direction Direction::operator*(const float s) const{
        return Direction(c[0] * s,
            c[1] * s, c[2] * s);
    }
    //direction / scalar
    Direction Direction::operator/(const float s) const{
        return Direction(c[0] / s,
            c[1] / s, c[2] / s);
    }

    float Direction::modulus() const{
        return sqrt( pow(c[0], 2) +
            pow(c[1], 2) + pow(c[1], 2) );
    }

    Direction Direction::normalize() const{
        float mod = modulus();
        return Direction(c[0] / mod,
            c[1] / mod, c[2] / mod);
    }

    float Direction::dotProd(const Direction &d) const{
        return c[0] * d.c[0] +
            c[1] * d.c[1] + c[2] * d.c[2];
    }
    Direction Direction::crossProd(const Direction &d) const{
        return Direction(
            c[1] * d.c[2] - c[2] * d.c[1],
            c[2] * d.c[0] - c[0] * d.c[2],
            c[0] * d.c[1] - c[1] * d.c[0]);
    }

    ostream& operator<<(std::ostream& os, const Direction& d){
 	os << "{" << d.c[0] << ", " << d.c[1] << ", " << d.c[2] << "}";

	return os;
}