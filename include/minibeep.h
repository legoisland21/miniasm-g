#ifndef MINIBEEP_H
#define MINIBEEP_H
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>

using namespace std;

#ifdef _WIN32
#include <windows.h>
inline void beep(double freq, int ms) {
    Beep((DWORD)freq, (DWORD)ms);
}
#else
#include <unistd.h>
inline void beep(double freq, int ms) {
    for(int i = 0; i < ms; i++) {
        putchar(7);
        fflush(stdout);
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
#endif

#endif