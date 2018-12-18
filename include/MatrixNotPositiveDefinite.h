//
// Created by Olcay Taner Yıldız on 26.11.2018.
//

#ifndef MATH_MATRIXNOTPOSITIVEDEFINITE_H
#define MATH_MATRIXNOTPOSITIVEDEFINITE_H
#include <exception>
using namespace std;

class MatrixNotPositiveDefinite : public exception{
public:
    const char* what() const noexcept override;
};


#endif //MATH_MATRIXNOTPOSITIVEDEFINITE_H
