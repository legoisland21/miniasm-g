#include "include/miniasm.h"
using namespace std;

vector<Register> registers;
AsciiTable game(64, 32);
vector<vector<int>> pixels(game.getWidth(), vector<int>(game.getHeight(), -1));
int x = game.getWidth() / 2, y = game.getHeight() / 2;
const int FPS = game.calculateDelta(5);

enum Direction { UP, RIGHT, DOWN, LEFT };
Direction dir = UP;

void doLogic() {
    if (pixels[x][y] == -1) {
        dir = static_cast<Direction>((dir + 1) % 4);
        pixels[x][y] = 1;
    } else {
        dir = static_cast<Direction>((dir + 3) % 4);
        pixels[x][y] = -1;
    }

    switch(dir) {
        case UP: y--; break;
        case RIGHT: x++; break;
        case DOWN: y++; break;
        case LEFT: x--; break;
    }

    if(x < 0) x = game.getWidth() - 1;
    if(x >= game.getWidth()) x = 0;
    if(y < 0) y = game.getHeight() - 1;
    if(y >= game.getHeight()) y = 0;
}

void drawCanvas() {
    for (int i = 0; i < game.getWidth(); i++) {
        for (int j = 0; j < game.getHeight(); j++) {
            if (pixels[i][j] == -1) {
                game.setPixel(i, j, ' ', BLACK, BLACK);
            } else {
                game.setPixel(i, j, '#', WHITE, WHITE);
            }
        }
    }
}

int main() {
    enableANSI();
    clearWindow();
    while(true) {
        game.clear(' ', WHITE);
        doLogic();
        drawCanvas();
        game.setPixel(x, y, '#', RED, RED);
        game.render();
        wait(FPS);
    }
}
