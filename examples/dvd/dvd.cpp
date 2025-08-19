#include "include/miniasm.h"

vector <Register> registers;
int x, y, rdir;
bool upD, downD, leftD, rightD;
bool wall;
int color = 7;

class Ball {
private:
    int x, y;
    bool upD, downD, leftD, rightD;
    int color;
public:
    Ball(int screenWidth, int screenHeight) {
        x = rand() % screenWidth;
        y = rand() % screenHeight;
        color = 7;

        int rdir = rand() % 4;
        upD = downD = leftD = rightD = false;
        if (rdir == 0) { upD = true; rightD = true; }
        else if (rdir == 1) { upD = true; leftD = true; }
        else if (rdir == 2) { downD = true; rightD = true; }
        else if (rdir == 3) { downD = true; leftD = true; }
    }

    void update(AsciiTable &screen) {
        bool wall = false;

        if (upD) y--;
        if (downD) y++;
        if (leftD) x--;
        if (rightD) x++;

        if (y == 0) { downD = true; upD = false; wall = true; }
        if (y == screen.getHeight() - 1) { upD = true; downD = false; wall = true; }
        if (x == 0) { rightD = true; leftD = false; wall = true; }
        if (x == screen.getWidth() - 1) { leftD = true; rightD = false; wall = true; }

        if (wall) {
            color = rand() % 7;
            beepns(A4, 250);
        }

        screen.setPixel(x, y, 'O', color, color);
    }
};

int main() {
    AsciiTable screen(64, 24);
    int delta = screen.calculateDelta(60);
    srand(time(NULL));
    vector<Ball> balls;
    for (int i = 0; i != 1; i++) balls.emplace_back(screen.getWidth(), screen.getHeight());
    while(true) {
        //screen.clear();
        for (auto &ball : balls) {
            ball.update(screen);
        }
        screen.render();
        wait(delta);
    }
}