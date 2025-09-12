#ifndef MINIAUDIO_H
#define MINIAUDIO_H

#include <string>
#include <thread>
#include <map>
#include <mutex>
#include <atomic>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

struct WAVData {
    int channels=0;
    int sampleRate=0;
    int bitsPerSample=0;
    vector<int16_t> samples;
    bool valid=false;
};

inline WAVData loadWav16(const string &path){
    WAVData out;
    ifstream f(path, ios::binary); if(!f) return out;
    char riff[4]; f.read(riff,4); if(strncmp(riff,"RIFF",4)!=0) return out;
    f.seekg(8); char wave[4]; f.read(wave,4); if(strncmp(wave,"WAVE",4)!=0) return out;
    while(f){
        char chunkId[4]; uint32_t chunkSize=0;
        f.read(chunkId,4); if(!f) break; f.read(reinterpret_cast<char*>(&chunkSize),4);
        if(strncmp(chunkId,"fmt ",4)==0){
            uint16_t fmt=0,nc=0; uint32_t sr=0,br=0; uint16_t ba=0,bps=0;
            f.read(reinterpret_cast<char*>(&fmt),2); f.read(reinterpret_cast<char*>(&nc),2);
            f.read(reinterpret_cast<char*>(&sr),4); f.read(reinterpret_cast<char*>(&br),4);
            f.read(reinterpret_cast<char*>(&ba),2); f.read(reinterpret_cast<char*>(&bps),2);
            if(fmt!=1) return out; out.channels=nc; out.sampleRate=sr; out.bitsPerSample=bps;
            if(chunkSize>16) f.seekg(chunkSize-16,ios::cur);
        } else if(strncmp(chunkId,"data",4)==0){
            if(out.bitsPerSample!=16) return out;
            size_t cnt=chunkSize/(out.bitsPerSample/8);
            out.samples.resize(cnt);
            f.read(reinterpret_cast<char*>(out.samples.data()),chunkSize);
            out.valid=true; break;
        } else f.seekg(chunkSize,ios::cur);
    }
    return out;
}

struct SoundInstance {
    atomic<bool> playing{true};
    atomic<bool> stopFlag{false};
    thread worker;
};

#ifdef _WIN32
map<string,shared_ptr<SoundInstance>> audioCache;
mutex audioMutex;

inline void playAudio(const string &path){
    WAVData wav=loadWav16(path); if(!wav.valid) return;
    auto inst=make_shared<SoundInstance>();
    {
        lock_guard<mutex> lk(audioMutex); audioCache[path]=inst;
    }
    inst->worker=thread([inst,wav,path](){
        string alias="music_"+to_string(reinterpret_cast<uintptr_t>(inst.get()));
        string cmd="open \""+path+"\" type waveaudio alias "+alias;
        mciSendStringA(cmd.c_str(),nullptr,0,nullptr);
        cmd="play "+alias+" wait";
        mciSendStringA(cmd.c_str(),nullptr,0,nullptr);
        cmd="close "+alias;
        mciSendStringA(cmd.c_str(),nullptr,0,nullptr);
        inst->playing.store(false);
        lock_guard<mutex> lk(audioMutex); audioCache.erase(path);
    });
    inst->worker.join();
}

inline void playAudioNs(const string &path){
    WAVData wav=loadWav16(path); if(!wav.valid) return;
    auto inst=make_shared<SoundInstance>();
    {
        lock_guard<mutex> lk(audioMutex); audioCache[path]=inst;
    }
    inst->worker=thread([inst,wav,path](){
        string alias="music_"+to_string(reinterpret_cast<uintptr_t>(inst.get()));
        string cmd="open \""+path+"\" type waveaudio alias "+alias;
        mciSendStringA(cmd.c_str(),nullptr,0,nullptr);
        cmd="play "+alias+" wait";
        mciSendStringA(cmd.c_str(),nullptr,0,nullptr);
        cmd="close "+alias;
        mciSendStringA(cmd.c_str(),nullptr,0,nullptr);
        inst->playing.store(false);
        lock_guard<mutex> lk(audioMutex); audioCache.erase(path);
    });
    inst->worker.detach();
}

inline bool isPlaying(const string &path){
    lock_guard<mutex> lk(audioMutex);
    auto it=audioCache.find(path);
    return it!=audioCache.end() && it->second->playing.load();
}

inline void stopPlaying(const string &path){
    lock_guard<mutex> lk(audioMutex);
    auto it=audioCache.find(path);
    if(it!=audioCache.end()) it->second->stopFlag.store(true);
}

inline void stopAll(){
    lock_guard<mutex> lk(audioMutex);
    for(auto &p:audioCache) p.second->stopFlag.store(true);
}
#else
#include <alsa/asoundlib.h>
inline map<string,shared_ptr<SoundInstance>> linuxCache;
inline mutex audioMutex;

inline void playAudio(const string &path){
    WAVData wav=loadWav16(path); if(!wav.valid) return;
    auto inst=make_shared<SoundInstance>();
    {lock_guard<mutex> lk(audioMutex); linuxCache[path]=inst;}
    inst->worker=thread([inst,wav,path](){
        snd_pcm_t *pcm;
        snd_pcm_open(&pcm,"default",SND_PCM_STREAM_PLAYBACK,0);
        snd_pcm_set_params(pcm,SND_PCM_FORMAT_S16_LE,SND_PCM_ACCESS_RW_INTERLEAVED,wav.channels,wav.sampleRate,1,500000);
        size_t pos=0;
        while(!inst->stopFlag.load() && pos<wav.samples.size()){
            size_t chunk=min<size_t>(1024,wav.samples.size()-pos);
            snd_pcm_writei(pcm,wav.samples.data()+pos,chunk/wav.channels);
            pos+=chunk;
        }
        snd_pcm_drain(pcm); snd_pcm_close(pcm);
        inst->playing.store(false);
        lock_guard<mutex> lk(audioMutex); linuxCache.erase(path);
    });
    inst->worker.join();
}

inline void playAudioNs(const string &path){ playAudio(path); }

inline bool isPlaying(const string &path){
    lock_guard<mutex> lk(audioMutex);
    auto it=linuxCache.find(path);
    return it!=linuxCache.end() && !it->second->stopFlag.load();
}

inline void stopPlaying(const string &path){
    lock_guard<mutex> lk(audioMutex);
    auto it=linuxCache.find(path);
    if(it!=linuxCache.end()) it->second->stopFlag.store(true);
}

inline void stopAll(){
    lock_guard<mutex> lk(audioMutex);
    for(auto &p:linuxCache) p.second->stopFlag.store(true);
}
#endif

#endif
