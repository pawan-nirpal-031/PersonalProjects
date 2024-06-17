clang-format -i digits.cpp
clang-format -i mathutils.h
clang-format -i neuralnet.h
g++ -O3 -g digits.cpp -o digits
./digits