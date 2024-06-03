
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>

#define LENGTH_ERR "Invalid matrix size"
#define OUT_OF_RANGE_ERR "Invalid matrix size"
#define VALID_VALUE 0.1
#define IMAGE_READ_ERROR "Error occurred while reading image"
#define ZERO_F 0.f
#define ZERO 0
#define ONE 1


//typedef struct matrix_dims
//{
//    int rows;
//    int cols;
//
//}matrix_dims;

class Matrix {

 public:
    /**
     * @struct dims
     * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
     */
     Matrix(int rows, int cols);
     Matrix();
     Matrix(Matrix const & matrix);
     ~Matrix();

     int get_rows() const;
     int get_cols() const;

     Matrix & transpose();
     Matrix & vectorize();
     void plain_print();
     Matrix dot(Matrix & matrix) const;
     float sum() const;
     float norm() const;
     int argmax() const;

     Matrix & operator += (const Matrix & matrix);
     Matrix operator + (const Matrix & matrix) const;
     Matrix & operator = (const Matrix & matrix);
     Matrix operator * (const Matrix & matrix) const;
     Matrix operator * (float c) const;
     friend Matrix operator * (float c, const Matrix & matrix);

     float operator () (int row, int col) const;
     float operator [] (int _i) const;
     float & operator () (int row, int col);
     float & operator [] (int _i);


     friend std::ostream &operator << (std::ostream &out,
         const Matrix &matrix);
     friend std::istream &operator >> (std::istream &in, Matrix &matrix);

  typedef struct dims
  {
      int rows;
      int cols;

  }dims;



 private:

  float ** _matrix;
  dims _dims;
  static void init_matrix (float ***mat, const dims &_dims, float val);
  static void free_matrix(float *** matrix, const int & rows);
  static void copy_matrix (float **src_mat, float **dst_mat,
                           const dims &_dims);

};

#endif //MATRIX_H
