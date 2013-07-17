if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")
endif (UNIX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(mmu_SOURCES
  main.cpp
  pte.cpp
  mmu.cpp
  fault_handler.cpp
  pager_with_free_list.cpp
  pager_with_frame_table.cpp
  pager_random.cpp
  pager_fifo.cpp
  pager_second_chance.cpp
  pager_clock.cpp
)

add_executable(mmu ${mmu_SOURCES})
