if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")
endif (UNIX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(mmu_SOURCES main.cpp pte.cpp mmu.cpp fault_handler.cpp pagers.cpp)

add_executable(mmu ${mmu_SOURCES})
