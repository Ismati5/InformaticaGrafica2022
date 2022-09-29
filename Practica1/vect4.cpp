/**
 * @file vect4.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-09-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "vect4.hpp"

/**
 * @brief operator<<
 *
 * @param os
 * @param v
 * @return ostream&
 */
ostream &operator<<(ostream &os, const Vect4 &v)
{
    os << "{" << v.v[0] << ", " << v.v[1] << ", " << v.v[2] << ", " << v.v[3] << "}";

    return os;
};