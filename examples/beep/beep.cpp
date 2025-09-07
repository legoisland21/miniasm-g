#include "include/miniasm.h"

vector<Register> registers;

void jungleHunt() {
    beepns(G5_SHARP, 130);
    wait(130);
    beepns(A5_SHARP, 130);
    wait(130);
    wait(130);
    beepns(G5_SHARP, 130);
    wait(130);
    beepns(B5, 130);
    wait(130);
    beepns(A5_SHARP, 130);
    wait(130);
    beepns(G5_SHARP, 130);
    wait(130);
    beepns(F5, 130);
    wait(130);
    wait(130);
    beepns(G5_SHARP, 130);
    wait(130);
    beepns(F5, 130);
    wait(130);
    beepns(F5_SHARP, 130);
    wait(130);
    beepns(F5, 130);
    wait(130);
    beepns(D5_SHARP, 130);
    wait(130);
    beepns(F5, 130);
    wait(130);
    beepns(D5_SHARP, 130);
    wait(130);
    beepns(C5_SHARP, 260);
    wait(260);
    wait(130);
    beepns(C6_SHARP, 391);
    wait(391);
}

int main() {
    cout << "Jungle hunt!" << endl;
    jungleHunt();
}
