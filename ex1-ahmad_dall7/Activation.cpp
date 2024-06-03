//
// Created by ahdal_9lda2pd on 21/12/2022.
//

#include "Activation.h"


Matrix activation::relu (const Matrix &matrix)
{
  Matrix result(matrix);

  for(int i=0 ; i<matrix.get_rows() ; ++i)
  {
    for (int j=0 ; j<matrix.get_cols() ; ++j)
    {
      result(i, j) = matrix(i, j) >= 0 ?
          matrix(i, j) : 0;
    }
  }
  return result;
}

Matrix activation::softmax (const Matrix &matrix)
{
  Matrix result (matrix);
  float s = ZERO;
  for (int i = 0 ; i<matrix.get_rows () ; ++i)
  {
    for (int j=0 ; j<matrix.get_cols() ; ++j)
    {
      result(i, j) = std::exp (matrix(i, j));
      s += result(i, j);
    }
  }
  return result * (1 / s);
}
