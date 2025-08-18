#ifndef MINIBEEP_H
#define MINIBEEP_H

#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cmath>
#include <cstdio>

using namespace std;

static vector<thread> g_threads;
static mutex g_mutex;

inline double two_pi() { return 6.28318530717958647692; }

#ifdef _WIN32
#include <windows.h>
inline void beep(double freq, int ms) {
    const int sampleRate = 44100;
    const int samples = (sampleRate * ms) / 1000;
    vector<short> buffer(samples);
    const double w = two_pi() * freq / sampleRate;
    const short amp = 8000; // quieter output
    for (int i = 0; i < samples; ++i) {
        buffer[i] = (short)(amp * sin(w * i));
    }

    WAVEFORMATEX wf = {};
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 1;
    wf.nSamplesPerSec = sampleRate;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

    HWAVEOUT hWaveOut;
    if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) return;

    WAVEHDR hdr = {};
    hdr.lpData = (LPSTR)buffer.data();
    hdr.dwBufferLength = samples * sizeof(short);
    waveOutPrepareHeader(hWaveOut, &hdr, sizeof(hdr));
    waveOutWrite(hWaveOut, &hdr, sizeof(hdr));
    while (waveOutUnprepareHeader(hWaveOut, &hdr, sizeof(hdr)) == WAVERR_STILLPLAYING) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    waveOutClose(hWaveOut);
}
#else
inline void beep(double freq, int ms) {
    const int sampleRate = 44100;
    const int samples = (sampleRate * ms) / 1000;
    FILE* pipe = popen("aplay -q -f S16_LE -r 44100 -t raw > /dev/null 2>&1", "w");
    if (!pipe) return;
    const double w = two_pi() * freq / sampleRate;
    const short amp = 8000;
    for (int i = 0; i < samples; ++i) {
        short s = (short)(amp * sin(w * i));
        fwrite(&s, sizeof(short), 1, pipe);
    }
    pclose(pipe);
}
#endif

inline void beepns(double freq, int ms) {
    lock_guard<mutex> lk(g_mutex);
    g_threads.emplace_back([freq, ms]() {
        beep(freq, ms);
    });
}

inline void beep_wait_all() {
    lock_guard<mutex> lk(g_mutex);
    for (auto& t : g_threads) {
        if (t.joinable()) t.join();
    }
    g_threads.clear();
}

struct AutoJoin {
    ~AutoJoin() { beep_wait_all(); }
};
static AutoJoin _autoJoin;

#endif