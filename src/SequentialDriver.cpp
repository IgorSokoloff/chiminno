#include "Solver.h"
#include "Vec.h"
#include <chrono>
#include <iostream>

namespace {

constexpr unsigned DIMENSIONS = 1000;
constexpr unsigned ITERATIONS = 100;
constexpr unsigned INEQUALITIES_COUNT = 8000;

} // namespace

int main(int argc, char *argv[]) {
  cimmpino::System Inequalities(/*Size=*/INEQUALITIES_COUNT,
                                /*Dimensions=*/DIMENSIONS);
  const auto TimerStart = std::chrono::high_resolution_clock::now();
  cimmpino::SequentialSolver Cimmino(std::move(Inequalities));
  cimmpino::Vec Result = Cimmino.solve(/*Iterations=*/ITERATIONS);
  const auto TimerEnd = std::chrono::high_resolution_clock::now();
  std::cout << "Process terminated in "
            << std::chrono::duration_cast<std::chrono::milliseconds>(TimerEnd -
                                                                     TimerStart)
                   .count()
            << " ms";
}
