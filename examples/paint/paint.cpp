#include <iostream>
#include <vector>
#include "include/miniasm.h"

vector<Register> registers;
AsciiTable game(96, 32);

vector<vector<int>> pixels(game.getWidth(), vector<int>(game.getHeight() - 1, -1)); 

int x, y;

int currentColor = BLACK, backgroundColor = BLACK;

void drawCanvas() {
    for (int x = 0; x < game.getWidth(); x++) {
        for (int y = 0; y < game.getHeight() - 1; y++) {
            if (pixels[x][y] == -1) {
                game.setPixel(x, y, ' ', backgroundColor, backgroundColor);
            } else {
                game.setPixel(x, y, ' ', pixels[x][y], pixels[x][y]);
            }
        }
    }
}

void clearCanvas() {
    for (int x = 0; x < game.getWidth(); x++) {
        for (int y = 0; y < game.getHeight() - 1; y++) {
            pixels[x][y] = -1;
            game.setPixel(x, y, ' ', backgroundColor, backgroundColor);
        }
    }
}

void drawUI() {
    for(int i = 0; i != game.getWidth(); i++) {
        game.setPixel(i, game.getHeight() - 1, '#', BLACK, BLACK);
    }

    game.drawText(0, game.getHeight() - 1, "FG: ", WHITE, BLACK);

    game.setPixel(4, game.getHeight() - 1, '#', BLACK, BLACK);
    game.setPixel(5, game.getHeight() - 1, '#', RED, RED);
    game.setPixel(6, game.getHeight() - 1, '#', GREEN, GREEN);
    game.setPixel(7, game.getHeight() - 1, '#', YELLOW, YELLOW);
    game.setPixel(8, game.getHeight() - 1, '#', BLUE, BLUE);
    game.setPixel(9, game.getHeight() - 1, '#', MAGENTA, MAGENTA);
    game.setPixel(10, game.getHeight() - 1, '#', CYAN, CYAN);
    game.setPixel(11, game.getHeight() - 1, '#', WHITE, WHITE);

    game.drawText(13, game.getHeight() - 1, "BG: ", WHITE, BLACK);

    game.setPixel(17, game.getHeight() - 1, '#', BLACK, BLACK);
    game.setPixel(18, game.getHeight() - 1, '#', RED, RED);
    game.setPixel(19, game.getHeight() - 1, '#', GREEN, GREEN);
    game.setPixel(20, game.getHeight() - 1, '#', YELLOW, YELLOW);
    game.setPixel(21, game.getHeight() - 1, '#', BLUE, BLUE);
    game.setPixel(22, game.getHeight() - 1, '#', MAGENTA, MAGENTA);
    game.setPixel(23, game.getHeight() - 1, '#', CYAN, CYAN);
    game.setPixel(24, game.getHeight() - 1, '#', WHITE, WHITE);

    game.drawText(26, game.getHeight() - 1, "Color: ", WHITE, BLACK);
    game.setPixel(33, game.getHeight() - 1, '#', currentColor, currentColor); 

    game.drawText(35, game.getHeight() - 1, "Clear canvas: ", WHITE, BLACK);
    game.setPixel(49, game.getHeight() - 1, 'O', WHITE, BLACK);
}

void inputLogic() {
    if(y == game.getHeight() - 1) {
        if(x == 5) currentColor = RED;
        else if(x == 6) currentColor = GREEN;
        else if(x == 7) currentColor = YELLOW;
        else if(x == 8) currentColor = BLUE;
        else if(x == 9) currentColor = MAGENTA;
        else if(x == 10) currentColor = CYAN;
        else if(x == 11) currentColor = WHITE;
        else if(x == 4) currentColor = BLACK;

        if(x == 18) backgroundColor = RED;
        else if(x == 19) backgroundColor = GREEN;
        else if(x == 20) backgroundColor = YELLOW;
        else if(x == 21) backgroundColor = BLUE;
        else if(x == 22) backgroundColor = MAGENTA;
        else if(x == 23) backgroundColor = CYAN;
        else if(x == 24) backgroundColor = WHITE;
        else if(x == 17) backgroundColor = BLACK;

        if(x == 49) clearCanvas();
    } else {
        pixels[x][y] = currentColor;
    }
}

int main() {
    enableANSI();
    clearWindow();
    while(true) {
        game.clear(' ', backgroundColor);
        drawUI();

        int input = getInput();

        if(input == 'w') { if(y > 0) y--; }
        else if(input == 's') { if(y < game.getHeight() - 1) y++; }
        else if(input == 'd') { if(x < game.getWidth() - 1) x++; }
        else if(input == 'a') { if(x > 0) x--; }
        if(input == 32) inputLogic();

        drawCanvas();

        game.setPixel(x, y, '^', WHITE, game.getPixelBgColor(x, y));

        game.render();
        cout << currentColor << " " << backgroundColor << endl;
    }
}