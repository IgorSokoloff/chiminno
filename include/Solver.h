#pragma once

#include "Vec.h"
#include <string>

namespace cimmpino {

class System {
public:
  // Generates random inequality system where all coefficients are random floats
  // within [LowerBound, UpperBound] range.
  System(unsigned Size = 10000, unsigned Dimensions = 10000,
         const float LowerBound = -500, const float UpperBound = 500);
  // Deserializes inequality system from disk.
  System(const std::string &Filename);

  // Dumps inequality system to file on disk.
  void serialize(const std::string &Filename) const;

  const unsigned Dimensions;

  const std::vector<std::pair<Vec, float>> data() const;

private:
  std::vector<std::pair<Vec, float>> Inequalities;
};

class SequentialSolver {
public:
  SequentialSolver(System &&Inequalities, float Rate = 0.5);

  Vec solve(unsigned Iterations = 1000) const;

private:
  const unsigned Dimensions;
  const System Inequalities;
  const float Rate;
};

} // namespace cimmpino
