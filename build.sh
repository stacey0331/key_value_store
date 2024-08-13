PYBIND11_DIR=$(python3 -m pybind11 --cmakedir)
export pybind11_DIR="$PYBIND11_DIR" # accessible by child process

[ -d build ] && rm -r build
[ -f app/key_value_store_module*.so ] && rm app/key_value_store_module*.so

mkdir build && \
cd build && \
cmake -DBUILD_PYBIND=ON -DBUILD_TESTS=OFF .. && \
cmake --build . && \
cd .. && \
cp build/key_value_store_module*.so app && \
fastapi dev app/main.py