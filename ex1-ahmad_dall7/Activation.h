#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

// Insert activation namespace here...


typedef Matrix (*activation_fn) (const Matrix &) ;

namespace activation
{
    Matrix relu(const Matrix & matrix);
    Matrix softmax(const Matrix & matrix);
}




#endif //ACTIVATION_H