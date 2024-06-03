//
// Created by ahdal_9lda2pd on 21/12/2022.
//
#include "Dense.h"

Dense::Dense(Matrix& weight, Matrix& bias, activation_fn activation)
    : _weights(weight), _bias(bias), activation(activation) {}
const Matrix &Dense::get_weights () const
{
  return this->_weights;
}
const Matrix &Dense::get_bias () const
{
  return this->_bias;
}
const activation_fn &Dense::get_activation () const
{
  return this->activation;
}
Matrix Dense::operator()(const Matrix& matrix) const
{
  return activation(_weights * matrix + _bias);

}




