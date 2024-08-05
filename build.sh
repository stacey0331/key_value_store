PYBIND11_DIR=$(python3 -m pybind11 --cmakedir)
export pybind11_DIR="$PYBIND11_DIR" # accessible by child process

rm -r build
mkdir build
cd build
cmake .. 
cmake --build .
cd ..
rm app/key_value_store_module*.so
cp build/key_value_store_module*.so app
fastapi dev app/main.py