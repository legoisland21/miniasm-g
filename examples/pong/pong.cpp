#include "include/miniasm.h"
#include <time.h>
using namespace std;

vector<Register> registers;
bool upD, downD, leftD, rightD = true;

void riff() {
    for(int i = 0; i != 2; i++) {
        beepns(C4, 100);
        wait(100);
        beepns(E4, 100);
        wait(100);
        beepns(G4, 100);
        wait(100);
    }
}

int main() {
    enableANSI();
    srand(time(NULL));
    int y = rand() % 15;
    int bX = 1, bY = y + 1;
    int score = 0;
    const int fps = 60;
    int begin = rand() % 2;
    if(begin == 0) downD = true;
    else if(begin == 1) upD = true;

    AsciiTable game(48, 16);
    int delta = game.calculateDelta(fps);
    while(true) {
        game.clear(' ', BLACK);

        if(bX == 0) {
            beep(C4, 200); 
            beep(B3, 200); 
            beep(A3_SHARP, 200); 
            return 0;
        }
        
        // Ball movement: update position according to direction flags
        bY += downD ? 1 : -1;
        bX += rightD ? 1 : -1;

        // Bounce off top
        if(bY <= 0) {
            bY = 0; 
            downD = true; 
            beepns(C5, 250);
        }

        // Bounce off bottom
        if(bY >= game.getHeight() - 1) {
            bY = game.getHeight() - 1; 
            downD = false; 
            beepns(C5, 250);
        }

        // Bounce off right wall
        if(bX >= game.getWidth() - 1) {
            bX = game.getWidth() - 1; 
            rightD = false; 
            beepns(C5, 250);
        }

        // Paddle collision (paddle is at x=0, 3 chars tall)
        if(bX == 1 && bY >= y-1 && bY <= y+1) {
            score++;
            rightD = true;
            thread t(riff);
            t.detach();
        }
        
        char input = getInputNs();
        if(input != -1) {
            if(input == 'w' && y > 1) y--;
            if(input == 's' && y < game.getHeight() - 2) y++;
        }
        // Draw paddle (# characters stacked vertically at x=0)
        for(int i=-1; i<=1; i++) game.setPixel(0, y+i, '#', WHITE, BLACK);

        game.setPixel(bX, bY, 'O', BLUE, BLACK);

        game.drawText(0, 0, "Score: " + to_string(score), YELLOW, BLACK);

        game.render();
        wait(delta);
    }
}