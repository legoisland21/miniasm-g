#include <iostream>
#include <vector>
#include "include/miniasm.h"

using namespace std;
vector<Register> registers;

int main() {
    if(dirExists("tools")) cout << "67" << endl;
    if(!fileExists("audio.wav")) {
        cout << "audio.wav not found!" << endl;
        return 1;
    }
    cout << "Blocking audio" << endl;
    playAudio("audio.wav");
    cout << "Non-blocking audio" << endl;
    playAudioNs("audio.wav");
    cout << "Already skipped!" << endl;
    cout << "Waiting 10 seconds for audio to finish" << endl;
    wait(10000);
    return 0;
}