add_library(Common

  Solver.cpp
  Vec.cpp
  )

add_executable(cimmpino

  ParallelDriver.cpp
  )
target_link_libraries(cimmpino

  Common)

add_executable(sequential-solver

  SequentialDriver.cpp
  )
target_link_libraries(sequential-solver

  Common)
