#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

// Insert Dense class here...

class Dense
{
 public:
  Dense (Matrix & weights, Matrix & bias, activation_fn func_type);
  const Matrix & get_weights() const;
  const Matrix & get_bias() const;
  const activation_fn & get_activation() const;
  Matrix operator()(const Matrix & matrix) const;

 private:
  Matrix _weights;
  Matrix _bias;
  activation_fn activation;
};



#endif //DENSE_H
