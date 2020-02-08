#include "Solver.h"
#include "Vec.h"
#include <chrono>
#include <iostream>
#include <mpi.h>

namespace {

constexpr unsigned MASTER = 0;
constexpr unsigned DIMENSIONS = 1000;
constexpr unsigned ITERATIONS = 100;
constexpr unsigned INEQUALITIES_COUNT = 8000;
constexpr float RATE = 0.5;

void sendToWorkers(cimmpino::Vec Vector) {
  for (int WorkerID = 1; WorkerID < MPI::COMM_WORLD.Get_size(); ++WorkerID)
    MPI::COMM_WORLD.Send(Vector.data(), DIMENSIONS, MPI::FLOAT, WorkerID, 0);
}

void updateResult(cimmpino::Vec &Result) {
  cimmpino::Vec Buffer(DIMENSIONS);
  for (int WorkerID = 1; WorkerID < MPI::COMM_WORLD.Get_size(); ++WorkerID) {
    MPI::COMM_WORLD.Recv(Buffer.data(), DIMENSIONS, MPI::FLOAT, WorkerID, 0);
    Result += Buffer;
  }
}

void updateEstimate(cimmpino::Vec &Result) {
  MPI::COMM_WORLD.Recv(Result.data(), DIMENSIONS, MPI::FLOAT, MASTER, 0);
}

void sendToMaster(cimmpino::Vec &Update) {
  MPI::COMM_WORLD.Send(Update.data(), DIMENSIONS, MPI::FLOAT, MASTER, 0);
}

void master(unsigned Workers) {
  const auto TimerStart = std::chrono::high_resolution_clock::now();
  cimmpino::Vec Result(DIMENSIONS);
  for (unsigned Iteration = 0; Iteration < ITERATIONS; ++Iteration) {
    sendToWorkers(Result);
    updateResult(Result);
  }
  const auto TimerEnd = std::chrono::high_resolution_clock::now();
  std::cout << "Process terminated in "
            << std::chrono::duration_cast<std::chrono::milliseconds>(TimerEnd -
                                                                     TimerStart)
                   .count()
            << " ms";
}

void worker(unsigned Rank, unsigned Workers, cimmpino::System &Inequalities) {
  cimmpino::Vec Result(DIMENSIONS);
  cimmpino::Vec Update(DIMENSIONS);
  for (unsigned Iteration = 0; Iteration < ITERATIONS; ++Iteration) {
    if (Iteration > 0)
      Update.zero();
    updateEstimate(Result);
    // Calculate projections and sum them in Update.
    for (auto Inequality : Inequalities.data()) {
      cimmpino::Vec Coefficients = Inequality.first;
      const float Bias = Inequality.second;
      const float ProjectionCoefficient =
          (Bias - Coefficients * Result) / Coefficients.squared_norm();
      Coefficients *= ProjectionCoefficient;
      Update += Coefficients;
    }
    Update *= RATE / INEQUALITIES_COUNT;
    // Return control MASTER.
    sendToMaster(Update);
  }
}

} // namespace

int main(int argc, char *argv[]) {
  MPI::Init(argc, argv);
  auto World = MPI::COMM_WORLD;
  const auto Rank = World.Get_rank();
  const int Workers = World.Get_size() - 1;
  if (Workers <= 0)
    return -1;
  cimmpino::System Inequalities(/*Size=*/INEQUALITIES_COUNT / Workers,
                                /*Dimensions=*/DIMENSIONS);
  if (Rank == MASTER)
    master(Workers);
  else
    worker(Rank, Workers, Inequalities);
  MPI::Finalize();
}
