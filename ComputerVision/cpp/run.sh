clang-format -i /home/panirpal/workspace/Projects/ComputerVision/cpp/src/BasicImageProcessing.cpp
cmake . && make -j32 
mv machineVision build
cd build 
./machineVision
cd - 
