#ifndef MINIAUDIO_H
#define MINIAUDIO_H

#include <string>
#include <thread>
#include <map>
#include <mutex>
#include <atomic>
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

struct SoundInstance {
    atomic<bool> playing{true};
    atomic<bool> stopFlag{false};
};

#ifdef _WIN32

static map<string, shared_ptr<SoundInstance>> audioCache;
static mutex audioMutex;

static void playAudio(const string &path){
    auto inst=make_shared<SoundInstance>();
    {
        lock_guard<mutex> lk(audioMutex);
        audioCache[path]=inst;
    }

    string alias="music_"+to_string(reinterpret_cast<uintptr_t>(inst.get()));
    string cmd="open \""+path+"\" type waveaudio alias "+alias;
    mciSendStringA(cmd.c_str(),nullptr,0,nullptr);
    cmd="play "+alias+" wait";
    mciSendStringA(cmd.c_str(),nullptr,0,nullptr);
    cmd="close "+alias;
    mciSendStringA(cmd.c_str(),nullptr,0,nullptr);

    inst->playing.store(false);
    lock_guard<mutex> lk(audioMutex);
    audioCache.erase(path);
}

static void playAudioNs(const string &path){
    thread([path](){ playAudio(path); }).detach();
}

static bool isPlaying(const string &path){
    lock_guard<mutex> lk(audioMutex);
    auto it=audioCache.find(path);
    return it!=audioCache.end() && it->second->playing.load();
}

static void stopPlaying(const string &path){
    lock_guard<mutex> lk(audioMutex);
    auto it=audioCache.find(path);
    if(it!=audioCache.end()) it->second->stopFlag.store(true);
}

static void stopAll(){
    lock_guard<mutex> lk(audioMutex);
    for(auto &p:audioCache) p.second->stopFlag.store(true);
}

#else

static map<string, shared_ptr<SoundInstance>> audioCache;
static mutex audioMutex;
static bool sdlInitialized=false;

static void initSDL(){
    if(!sdlInitialized){
        if(SDL_Init(SDL_INIT_AUDIO)<0) cerr<<"SDL_Init failed\n";
        if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)<0) cerr<<"Mix_OpenAudio failed\n";
        sdlInitialized=true;
    }
}

static void playAudio(const string &path){
    initSDL();
    auto inst=make_shared<SoundInstance>();
    {
        lock_guard<mutex> lk(audioMutex);
        audioCache[path]=inst;
    }

    Mix_Chunk *chunk=Mix_LoadWAV(path.c_str());
    if(!chunk){ inst->playing.store(false); lock_guard<mutex> lk(audioMutex); audioCache.erase(path); return; }
    int channel=Mix_PlayChannel(-1, chunk, 0);
    while(channel!=-1 && !inst->stopFlag.load() && Mix_Playing(channel)) SDL_Delay(10);
    Mix_HaltChannel(channel);
    Mix_FreeChunk(chunk);

    inst->playing.store(false);
    lock_guard<mutex> lk(audioMutex);
    audioCache.erase(path);
}

static void playAudioNs(const string &path){
    thread([path](){ playAudio(path); }).detach();
}

static bool isPlaying(const string &path){
    lock_guard<mutex> lk(audioMutex);
    auto it=audioCache.find(path);
    return it!=audioCache.end() && it->second->playing.load();
}

static void stopPlaying(const string &path){
    lock_guard<mutex> lk(audioMutex);
    auto it=audioCache.find(path);
    if(it!=audioCache.end()) it->second->stopFlag.store(true);
}

static void stopAll(){
    lock_guard<mutex> lk(audioMutex);
    for(auto &p:audioCache) p.second->stopFlag.store(true);
}

#endif

#endif
