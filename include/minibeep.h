#ifndef MINIBEEP_H
#define MINIBEEP_H

#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cmath>

using namespace std;

static vector<thread> g_threads;
static mutex g_mutex;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef _WIN32
#include <windows.h>

inline void beep(double freq, int ms) {
    const int sampleRate = 44100;
    const int samples = (sampleRate * ms) / 1000;
    vector<short> buffer(samples);

    const double w = 2.0 * M_PI * freq / sampleRate;
    const short amp = 8000;

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

#else // Linux/others → SDL2 sine

#include <SDL2/SDL.h>

struct BeepData {
    int samplesLeft;
    double phase;
    double phaseInc;
    bool done;
};

inline void audioCallback(void* userdata, Uint8* stream, int len) {
    BeepData* data = (BeepData*)userdata;
    float* out = (float*)stream;
    int frames = len / sizeof(float);

    for (int i = 0; i < frames; i++) {
        if (data->samplesLeft > 0) {
            out[i] = 0.25f * sinf((float)data->phase);
            data->phase += data->phaseInc;
            if (data->phase > 2 * M_PI) data->phase -= 2 * M_PI;
            data->samplesLeft--;
        } else {
            out[i] = 0.0f;
            data->done = true;
        }
    }
}

inline void beep(double freq, int ms) {
    const int sampleRate = 44100;
    BeepData data;
    data.samplesLeft = (sampleRate * ms) / 1000;
    data.phase = 0.0;
    data.phaseInc = 2.0 * M_PI * freq / sampleRate;
    data.done = false;

    SDL_AudioSpec want{}, have{};
    want.freq = sampleRate;
    want.format = AUDIO_F32SYS;
    want.channels = 1;
    want.samples = 1024;
    want.callback = audioCallback;
    want.userdata = &data;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) return;
    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (!dev) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return;
    }

    SDL_PauseAudioDevice(dev, 0);

    while (!data.done) {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    SDL_CloseAudioDevice(dev);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
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
