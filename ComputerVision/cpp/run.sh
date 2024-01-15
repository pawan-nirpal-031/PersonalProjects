find /home/panirpal/workspace/Projects/ComputerVision/cpp -type f \( -name "*.cpp" -or -name "*.h" \) -exec clang-format -i {} \;

cmake . && make -j32
mv MachineVision build
cd build 
./MachineVision
cd - 
