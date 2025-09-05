#include <iostream>
#include <vector>
#include <random>
#include "include/miniasm.h"
using namespace std;
random_device rdn;
mt19937 ramdom(rdn());

vector <Register> registers;
AsciiTable game(16, 8);

const int delta = game.calculateDelta(10);

int randomInt(int min, int max) {
    static uniform_int_distribution<int> dist; // reused
    dist.param(uniform_int_distribution<int>::param_type{min, max});
    return dist(rng);
}

int cX = randomInt(1, 7), cY = -10, x = randomInt(1, 7);
int score;

void collision() {
    x = max(1, min(x, 14));
}

void renderMap() {
    for(int i = 0; i != 8; i++) {
        game.setPixel(0, i, '#', BLUE, BLUE);
        game.setPixel(15, i, '#', BLUE, BLUE);
    }
}

void doCar() {
    cY++;
    int move = randomInt(0, 5);
    if(move == 1) cX++;
    else if(move == 2) cX--;

    if(cY == 8) { 
        cY = -5;
        cX = randomInt(1, 7);
        score++;
    }

    cX = max(1, min(cX, 14));
}

void lose() {
    beep(C4, 200); 
    beep(B3, 200); 
    beep(A3_SHARP, 200); 
    game.clear(' ', BLACK);
    game.drawText(0, 0, "You lost!");
    game.drawText(0, 1, "Final score: ");
    game.drawText(0, 2, to_string(score));
    game.render();
    exit(0);
}

int main() {
    enableANSI();
    clearWindow();
    while(true) {
        game.clear(' ', GREEN);
        renderMap();

        int input = getInputNs();
        if(input != -1) {
            if(input == 'd') x++;
            else if(input == 'a') x--;
        }

        collision();

        doCar();

        if(cX == x && cY == 6) lose();

        game.drawText(0, 7, to_string(score), BLACK, YELLOW);

        game.setPixel(x, 6, '#', WHITE, GREEN);

        game.setPixel(cX, cY, '#', MAGENTA, GREEN);

        game.render();
        wait(delta);
    }
}