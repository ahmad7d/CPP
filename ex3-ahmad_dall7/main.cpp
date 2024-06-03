//
// Created by talsu on 08/06/2022.
//UPDATED 14/06
#include "string"
#include <iostream>
#include "assert.h"
#include <stdexcept>
#include "Dictionary.hpp"
#include "HashMap.hpp"

// Checks default constructor and methods.

int main ()
{
  HashMap<int, int> A;
  A.insert (5, 5);
  A.insert (2, 2);

  HashMap<int, int> B = A;

  std::cout << A.capacity() << std::endl;

  A.erase (5);

  std::cout << A.capacity() << std::endl;



}