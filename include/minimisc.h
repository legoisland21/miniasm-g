#ifndef MINIMISC_H
#define MINIMISC_H

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include "register.h"

using namespace std;

random_device dev;
mt19937 rng(dev());

extern vector<Register> registers;

#ifdef _WIN32
#include <windows.h>
inline void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#else

inline void enableANSI() {}
#endif

inline void randInt(int index, int min, int max) {
    static uniform_int_distribution<int> dist;
    dist.param(uniform_int_distribution<int>::param_type{min, max});
    registers[index].ivalue = dist(rng);
}

inline void wait(int ms) {
    if (ms <= 0) return;
    this_thread::sleep_for(chrono::milliseconds(ms));
}

inline void clearWindow() {
    cout << "\033[2J\033[H";
}

int getOS() {
    #if defined(_WIN32) || defined(_WIN64)
        return 0;
    #elif defined(__linux__)
        return 1;
    #else
        return -1;
    #endif
}

#endif
