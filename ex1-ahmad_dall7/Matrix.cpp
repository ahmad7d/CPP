//
// Created by ahdal_9lda2pd on 15/12/2022.
//

#include "Matrix.h"

#include <cmath>




void Matrix::free_matrix (float ***matrix, const int & rows)
{
  for (int i=0 ; i<rows ; ++i)
  {
    delete (*matrix)[i];
  }
  delete[] (*matrix);
  *matrix = NULL;
}

void Matrix::init_matrix (float ***mat, const dims &_dims, float val)
{
  if (_dims.rows <= ZERO || _dims.cols <= ZERO)
  {
    throw std::runtime_error (OUT_OF_RANGE_ERR);
  }
  (*mat) = new float *[_dims.rows];
  for (int i=0 ; i < _dims.rows ; ++i)
  {
    (*mat)[i] = new float [_dims.cols];
    for (int j=0 ; j < _dims.cols ; ++j)
    {
      (*mat)[i][j] = val;
    }
  }
}

void Matrix::copy_matrix (float **src_mat, float **dst_mat,
                          const dims &_dims)
{
  for (int i = 0; i < _dims.rows; ++i)
  {
    for (int j = 0; j < _dims.cols; ++j)
    {
      dst_mat[i][j] = src_mat[i][j];
    }
  }
}

Matrix::Matrix (int rows, int cols): _dims(dims{rows, cols})
{
  init_matrix (&_matrix, _dims, ZERO);
}

Matrix::Matrix() : Matrix(1, 1){}

Matrix::Matrix (const Matrix &matrix)
: Matrix (matrix._dims.rows, matrix._dims.cols)
{
  copy_matrix (matrix._matrix, this->_matrix,matrix._dims);
}

Matrix::~Matrix ()
{
  free_matrix (&_matrix, get_rows());
}

int Matrix::get_rows () const
{
  return _dims.rows;
}

int Matrix::get_cols () const
{
  return _dims.cols;
}

Matrix &Matrix::transpose ()
{
  float **result = nullptr;
  dims new_dims = dims{get_cols(), get_rows()};
  init_matrix (&result, new_dims, ZERO);
  for (int i=0 ; i<_dims.rows ; ++i)
  {
    for (int j=0 ; j<_dims.cols ; ++j)
    {
      result[j][i] = _matrix[i][j];
    }
  }
  free_matrix (&_matrix, get_rows());
  _matrix = result;

  int temp = _dims.cols;
  _dims.cols = _dims.rows;
  _dims.rows = temp;

  return *this;
}
Matrix &Matrix::vectorize ()
{
  float ** vec;
  dims new_dims{get_rows() * get_cols(), ONE};
  init_matrix (&vec, new_dims, ONE);
  for (int i=0 ; i<get_rows() ; ++i)
  {
    for (int j=0 ; j<get_cols() ; ++j)
    {
      vec[i * get_cols() + j][0] = _matrix[i][j];
    }
  }
  free_matrix (&_matrix, get_rows());
  _matrix = vec;
  this->_dims.rows = this->get_rows() * this->get_cols();
  this->_dims.cols = ONE;

  return *this;
}
void Matrix::plain_print ()
{
  for (int i=0 ; i<get_rows() ; ++i)
  {
    for (int j=0 ; j<get_cols() ; ++j)
    {
      std::cout << _matrix[i][j] << ' ';
    }
    std::cout << std::endl;
  }
}

Matrix Matrix::dot (Matrix & matrix) const
{
  int rows = get_rows();
  int cols = get_cols();
  Matrix result(rows, cols);
  if (rows == matrix.get_rows() && cols == matrix.get_cols())
  {
    for (int i=0 ; i<rows ; ++i)
    {
      for (int j=0 ; j<cols ; ++j)
      {
        result._matrix[i][j] = _matrix[i][j] * matrix._matrix[i][j];
      }
    }
  }
  else
  {
    throw std::length_error(LENGTH_ERR);
  }
  return result;
}

float Matrix::sum () const
{
  float result = 0;
  for(int i=0 ; i< this->get_rows() ; ++i)
  {
    for (int j=0 ; j< this->get_cols() ; ++j)
    {
      result += this->_matrix[i][j];
    }
  }
  return result;
}

float Matrix::norm () const
{
  float result = 0;
  for (int i=0 ; i< this->get_rows() ; ++i)
  {
    for (int j=0 ; j< this->get_cols() ; ++j)
    {
      result += (float )pow(_matrix[i][j], 2);
    }
  }
  return sqrt(result);
}

int Matrix::argmax () const
{
  return get_rows() * get_cols() - 1;
}
Matrix &Matrix::operator+= (const Matrix &matrix)
{
  *this = *this + matrix;
  return *this;
}

Matrix &Matrix::operator = (const Matrix &matrix)
{
  free_matrix (&_matrix, get_rows ());
  _dims = {matrix.get_rows (), matrix.get_cols ()};
  init_matrix (&_matrix, _dims, ZERO);
  copy_matrix (matrix._matrix, _matrix, _dims);
  return *this;
}


Matrix Matrix::operator+ (const Matrix &matrix) const
{
  Matrix result(matrix);
  if (this->get_rows() == matrix.get_rows() &&
  this->get_cols() == matrix.get_cols())
  {
    for (int i=0 ; i<this->get_rows() ; ++i)
    {
      for(int j=0 ; j< this->get_cols() ; ++j)
      {
        result._matrix[i][j] += _matrix[i][j];
      }
    }
  }
  else
  {
    throw std::length_error(LENGTH_ERR);
  }
  return result;
}

Matrix Matrix::operator* (const Matrix &matrix) const
{
  Matrix result (get_rows (), matrix.get_cols ());
  if (get_cols () != matrix.get_rows ())
    {
      throw std::length_error (LENGTH_ERR);
    }
  for (int i = 0; i < get_rows (); ++i)
    {
      for (int j = 0; j < matrix.get_cols (); ++j)
        {
          for (int k = 0; k < get_cols (); ++k)
            {
              result._matrix[i][j] += _matrix[i][k] * matrix._matrix[k][j];
            }
        }
    }
  return result;
}
Matrix Matrix::operator* (float c) const
{
  Matrix result(*this);
  for (int i = 0; i<get_rows(); ++i)
  {
    for (int j = 0; j<get_cols (); ++j)
    {
      result._matrix[i][j] *= c;
    }
  }
  return result;
}

Matrix operator* (float c, const Matrix &matrix)
{
  return matrix * c;
}
float Matrix::operator() (int row, int col) const
{
  if (row >= get_rows () || col >= get_cols () || row < ZERO || col < ZERO)
  {
    throw std::out_of_range (OUT_OF_RANGE_ERR);
  }
  return _matrix[row][col];
}

float & Matrix::operator() (int row, int col)
{
  if (row >= get_rows () || col >= get_cols () || row < ZERO || col < ZERO)
  {
    throw std::out_of_range (OUT_OF_RANGE_ERR);
  }
  return _matrix[row][col];
}


float Matrix::operator[] (int _i) const
{
  return _i < ZERO ?  throw std::out_of_range (OUT_OF_RANGE_ERR) :
  (*this)(_i / get_cols(), _i % get_cols());
}
float & Matrix::operator[] (int _i)
{
  return _i < ZERO ?  throw std::out_of_range (OUT_OF_RANGE_ERR) :
         (*this)(_i / get_cols(), _i % get_cols());
}


std::ostream &operator<< (std::ostream &out, const Matrix &matrix)
{
  for (int i=0 ; i<matrix.get_rows() ; ++i)
  {
    for (int j=0 ; j<matrix.get_cols() ; ++j)
    {
      if (matrix(i, j) > VALID_VALUE)
      {
        out << "**";
      }
      else
      {
        out << " ";
      }
    }
    out << std::endl;
  }
  return out;
}
std::istream &operator>> (std::istream &in, Matrix &matrix)
{
  int rows = matrix.get_rows();
  int cols = matrix.get_cols();
  auto *buff = new (std::nothrow) float[cols];
  for (int i = 0; i < rows; ++i)
  {
    in.read ((char *) buff, cols * sizeof (float));
    if (!in)
    {
      throw std::runtime_error (IMAGE_READ_ERROR);
    }
    for (int j = 0; j < cols; ++j)
    {
      matrix (i, j) = buff[j];
    }
  }
  delete[] buff;
  return in;
}

//int main()
//{
//  Matrix m = Matrix (2, 2);
//  m(0, 0) = 3;
//  m(0, 1) = 3;
//  m(1, 0) = 3;
//  m(1, 1) = 3;
//}