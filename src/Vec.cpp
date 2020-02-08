//===--- Vec.cpp - n-d array for inequality solver --------------*- C++ -*-===//

#include "Vec.h"
#include <cassert>
#include <numeric>

namespace cimmpino {

Vec::Vec(unsigned Dimensions) : Dimensions(Dimensions), Data(Dimensions, 0) {}

float &Vec::operator[](size_t Index) { return Data[Index]; }

float Vec::operator*(Vec &RHS) {
  assert(Dimensions == RHS.Dimensions && "Vector dimensions must match.");
  float Result = 0;
  for (unsigned I = 0; I < Dimensions; ++I)
    Result += Data[I] * RHS[I];
  return Result;
}

Vec &Vec::operator+=(Vec &RHS) {
  assert(Dimensions == RHS.Dimensions && "Vector dimensions must match.");
  for (unsigned I = 0; I < Dimensions; ++I)
    Data[I] += RHS[I];
  return *this;
}

Vec &Vec::operator*=(const float Factor) {
  for (unsigned I = 0; I < Dimensions; ++I)
    Data[I] *= Factor;
  return *this;
}

Vec &Vec::operator/=(const float Factor) { return *this *= 1. / Factor; }

float Vec::squared_norm() const {
  return std::accumulate(begin(Data), end(Data), 0.f,
                         [](float CurrentResult, float NextItem) {
                           return CurrentResult + NextItem * NextItem;
                         });
}

float Vec::squared_difference_norm(Vec &RHS) const {
  assert(Dimensions == RHS.Dimensions && "Vector dimensions must match.");
  float Result = 0;
  for (unsigned I = 0; I < Dimensions; ++I) {
    const float Difference = Data[I] - RHS[I];
    Result += Difference * Difference;
  }
  return Result;
}

float *Vec::data() { return Data.data(); }

void Vec::zero() {
  for (auto &Item : Data)
    Item = 0;
}

} // namespace cimmpino
