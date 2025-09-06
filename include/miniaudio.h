#ifndef MINIAUDIO_H
#define MINIAUDIO_H

#include <string>
#include <thread>
#include <cstdlib>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif

inline void playAudio(const string &path) {
#ifdef _WIN32
    string cmd = "open \"" + path + "\" type waveaudio alias music";
    mciSendStringA(cmd.c_str(), nullptr, 0, nullptr);
    mciSendStringA("play music wait", nullptr, 0, nullptr);
    mciSendStringA("close music", nullptr, 0, nullptr);
#else
    string cmd = "aplay \"" + path + "\" >/dev/null 2>&1";
    system(cmd.c_str());
#endif
}

inline void playAudioNs(const string &path) {
    thread([path]() {
        playAudio(path);
    }).detach();
}

#endif
