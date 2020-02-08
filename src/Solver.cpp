#include "Solver.h"
#include <cstdlib>
#include <iostream>

namespace cimmpino {

namespace {

float sampleRandomFloat(const float LowerBound, const float UpperBound) {
  // Random number within [0, 1] range.
  const float RandomScale =
      static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  const float RangeWidth = UpperBound - LowerBound;
  const float ScaledWidth = RangeWidth * RandomScale;
  return LowerBound + ScaledWidth;
}

} // namespace

System::System(unsigned Size, unsigned Dimensions, const float LowerBound,
               const float UpperBound)
    : Dimensions(Dimensions) {
  for (unsigned VecID = 0; VecID < Size; ++VecID) {
    cimmpino::Vec NextVector(Dimensions);
    for (unsigned ItemID = 0; ItemID < Dimensions; ++ItemID) {
      NextVector[ItemID] = sampleRandomFloat(LowerBound, UpperBound);
    }
    float Bias = sampleRandomFloat(LowerBound, UpperBound);
    Inequalities.emplace_back(NextVector, Bias);
  }
}

const std::vector<std::pair<Vec, float>> System::data() const {
  return Inequalities;
}

SequentialSolver::SequentialSolver(System &&Inequalities, float Rate)
    : Inequalities(std::move(Inequalities)),
      Dimensions(Inequalities.Dimensions), Rate(Rate) {}

Vec SequentialSolver::solve(unsigned Iterations) const {
  Vec Result(Dimensions);
  for (unsigned It = 0; It < Iterations; ++It) {
    Vec Projections(Dimensions);
    for (auto Inequality : Inequalities.data()) {
      Vec Coefficients = Inequality.first;
      const float Bias = Inequality.second;
      const float ProjectionCoefficient =
          (Bias - Coefficients * Result) / Coefficients.squared_norm();
      Coefficients *= ProjectionCoefficient;
      Projections += Coefficients;
    }
    Projections *= Rate / Inequalities.data().size();
    Result += Projections;
  }
  return Result;
}

} // namespace cimmpino
