#ifndef UTILS
#define UTILS
#include <iostream>
#include <vector>
#include <assert.h>
#include <chrono>
#include <cstdio>
#include <future>
#include <thread>
#ifdef _WIN32
#include <conio.h>
#elif __linux__
#include <termios.h>
#include <unistd.h>
#endif

enum cellType{
    EmptyT,
    WallT,
    PelletT,
    PowerPelletT,
    PacmanT,
    GhostT
};

enum class PelletType {
    Regular,
    Power
};

enum class Direction{
    Up,
    Down,
    Left,
    Right
};

enum class Signal{
    GameOver,
    StartNextLife
};
#endif