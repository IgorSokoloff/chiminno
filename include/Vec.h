//===--- Vec.h - n-d array for inequality solver ----------------*- C++ -*-===//
//
// Data structures for N-dimensional vector and mathematical operations over it
// and
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>

namespace cimmpino {

// Fixed-width n-dimensional array of single precision floating point numbers.
class Vec {
public:
  // Constructs zeroed vector.
  Vec(unsigned Dimensions);

  // Returns Data[Index].
  float &operator[](size_t Index);
  // Returns dot product of two vectors.
  float operator*(Vec &RHS);
  // Calculates the sum of two vectors and stores it in the LHS.
  Vec &operator+=(Vec &RHS);
  // Multiplies each value of the vector by given factor mutating the object.
  Vec &operator*=(const float Factor);
  // Divides each value of the vector by given factor mutating the object.
  Vec &operator/=(const float Factor);
  // Returns squared l_2 norm of *this.
  float squared_norm() const;
  // Returns squared l_2 norm difference between *this and RHS.
  float squared_difference_norm(Vec &RHS) const;
  float *data();

  void zero();

private:
  const unsigned Dimensions;
  // Holds single precision vector data.
  std::vector<float> Data;
};

} // namespace cimmpino
