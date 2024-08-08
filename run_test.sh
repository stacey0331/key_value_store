[ -d build ] && rm -r build
cmake -S . -B build -DBUILD_PYBIND=OFF -DBUILD_TESTS=ON
cmake --build build 
cd build 
ctest --rerun-failed --output-on-failure