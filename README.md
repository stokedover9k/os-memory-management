os-memory-management
====================

Memory Management lab for Franke's OS course

Compile
============================================

The project is managed with CMake and out-of-source build is ensured.
So, after extracting the project files, you must create a directory (call it 'build') in the root project directory and build from there.

unzip os-mmu.zip
cd os-mmu
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

Run
============================================

./mmu [-a<algo:aArfscClN>] [-o<options:OPpFfS>] [–f<num_frames>] inputfile randomfile
