//
// Created by Olcay Taner Yıldız on 26.11.2018.
//

#ifndef MATH_MATRIXROWCOLUMNMISMATCH_H
#define MATH_MATRIXROWCOLUMNMISMATCH_H
#include <exception>
using namespace std;

class MatrixRowColumnMismatch : public exception {
public:
    const char* what() const noexcept override;
};


#endif //MATH_MATRIXROWCOLUMNMISMATCH_H
