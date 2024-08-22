[ -d build ] && sudo rm -r build

brew install pkg-config
brew install libpq
brew install libpqxx

cmake -S . -B build -DBUILD_PYBIND=OFF -DBUILD_TESTS=ON
cmake --build build 
cd build 
ctest --rerun-failed --output-on-failure