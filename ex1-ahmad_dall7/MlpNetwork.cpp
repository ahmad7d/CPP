//
// Created by ahdal_9lda2pd on 21/12/2022.
//

#include "MlpNetwork.h"


MlpNetwork::MlpNetwork(Matrix weights[MLP_SIZE],
                       Matrix bias[MLP_SIZE]):layers_count(MLP_SIZE)
{
  layers = new (std::nothrow) Dense * [MLP_SIZE];
  for (int i = 0; i < MLP_SIZE; ++i)
  {
    layers[i] = new (std::nothrow) Dense (weights[i], bias[i],
                                          i == MLP_SIZE - 1
                                          ? activation::softmax
                                          : activation::relu);
  }
}

digit MlpNetwork::operator() (const Matrix &matrix) const
{
  Matrix result (matrix);
  result.vectorize();
  digit d{ZERO, ZERO_F};
  for (int i = 0; i < MLP_SIZE; ++i)
  {
    result = (*(this->layers[i])) (result);
  }

  for (int i = 0; i < result.get_rows (); ++i)
  {
    if (result[i] > d.probability)
    {
      d.probability = result[i];
      d.value = i;
    }
  }
  return d;
}
MlpNetwork::~MlpNetwork ()
{
  for(int i=0 ; i<layers_count ; ++i)
  {
    delete layers[i];
  }
  delete[] layers;
}

































