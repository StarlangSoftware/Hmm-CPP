//
// Created by Olcay Taner Yıldız on 4.12.2018.
//

#ifndef MATH_MATRIXNOTSQUARE_H
#define MATH_MATRIXNOTSQUARE_H
#include <exception>
using namespace std;

class MatrixNotSquare : public exception{
public:
    const char* what() const noexcept override;
};


#endif //MATH_MATRIXNOTSQUARE_H
