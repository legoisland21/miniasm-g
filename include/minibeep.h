#ifndef MINIBEEP_H
#define MINIBEEP_H

#include <thread>
#include <chrono>
#include <cmath>
#include <cstdio>

using namespace std;

#ifdef _WIN32
#include <windows.h>
inline void beep(double freq, int ms) {
    Beep((DWORD)freq, (DWORD)ms);
}
#else
inline void beep(double freq, int ms) {
    const int sampleRate = 44100;
    const int samples = sampleRate * ms / 1000;

    FILE* pipe = popen("aplay -q -f S16_LE -r 44100 -t raw > /dev/null 2>&1", "w");
    if(!pipe) return;

    for(int i = 0; i < samples; i++) {
        short sample = 32767 * sin(2 * M_PI * freq * i / sampleRate);
        fwrite(&sample, sizeof(short), 1, pipe);
    }
    pclose(pipe);
}
#endif

#endif