clang-format -i digits.cpp
clang-format -i mathutils.h
clang-format -i neuralnet.h
g++ -O0 -g digits.cpp -o digits
./digits