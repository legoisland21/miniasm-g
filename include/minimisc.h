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
    uniform_int_distribution<mt19937::result_type> random_number(min, max);
    registers[index].ivalue = random_number(rng);
}

inline void wait(int ms) {
    auto start = chrono::high_resolution_clock::now();
    auto end = start + chrono::milliseconds(ms);
    while (chrono::high_resolution_clock::now() < end) {

    }
}

inline void clearWindow() {
    cout << "\033[2J\033[H";
}

#endif
