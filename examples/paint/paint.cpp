#include <iostream>
#include <vector>
#include "include/miniasm.h"

vector<Register> registers;
AsciiTable game(96, 32);

vector<vector<int>> pixels(96, vector<int>(31, 0));

int x, y;

int currentColor = BLACK, backgroundColor = BLACK;

void drawCanvas() {
    for (int x = 0; x < 96; x++) {
        for (int y = 0; y < 31; y++) {
            if (pixels[x][y] == 0) {
                game.setPixel(x, y, ' ', backgroundColor, backgroundColor);
            } else {
                game.setPixel(x, y, '#', pixels[x][y], pixels[x][y]);
            }
        }
    }
}

void clearCanvas() {
    for (int x = 0; x < 96; x++) {
        for (int y = 0; y < 31; y++) {
            pixels[x][y] = 0;
            game.setPixel(x, y, ' ', backgroundColor, backgroundColor);
        }
    }
}

void drawUI() {
    for(int i = 0; i != game.getWidth(); i++) {
        game.setPixel(i, game.getHeight() - 1, '#', BLACK, BLACK);
    }

    game.setPixel(0, game.getHeight() - 1, '#', BLACK, BLACK);
    game.setPixel(1, game.getHeight() - 1, '#', RED, RED);
    game.setPixel(2, game.getHeight() - 1, '#', GREEN, GREEN);
    game.setPixel(3, game.getHeight() - 1, '#', YELLOW, YELLOW);
    game.setPixel(4, game.getHeight() - 1, '#', BLUE, BLUE);
    game.setPixel(5, game.getHeight() - 1, '#', MAGENTA, MAGENTA);
    game.setPixel(6, game.getHeight() - 1, '#', CYAN, CYAN);
    game.setPixel(7, game.getHeight() - 1, '#', WHITE, WHITE);

    game.drawText(9, game.getHeight() - 1, "Background: ", WHITE, BLACK);

    game.setPixel(22, game.getHeight() - 1, '#', BLACK, BLACK);
    game.setPixel(23, game.getHeight() - 1, '#', RED, RED);
    game.setPixel(24, game.getHeight() - 1, '#', GREEN, GREEN);
    game.setPixel(25, game.getHeight() - 1, '#', YELLOW, YELLOW);
    game.setPixel(26, game.getHeight() - 1, '#', BLUE, BLUE);
    game.setPixel(27, game.getHeight() - 1, '#', MAGENTA, MAGENTA);
    game.setPixel(28, game.getHeight() - 1, '#', CYAN, CYAN);
    game.setPixel(29, game.getHeight() - 1, '#', WHITE, WHITE);

    game.drawText(31, game.getHeight() - 1, "Current color: ", WHITE, BLACK);
    game.setPixel(47, game.getHeight() - 1, '#', currentColor, currentColor); 

    game.drawText(49, game.getHeight() - 1, "Clear canvas: ", WHITE, BLACK);
    game.setPixel(62, game.getHeight() - 1, 'O', WHITE, BLACK);
}

void inputLogic() {
    if(y == game.getHeight() - 1) {
        if(x > -1 && x < 8) currentColor = game.getPixelColor(x, game.getHeight() - 1);
        if(x > 21 && x < 30) backgroundColor = game.getPixelBgColor(x, game.getHeight() - 1);
        if(x == 62) clearCanvas();
    } else {
        game.setPixel(x, y, '#', currentColor, currentColor);
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
        else if(input == 32) inputLogic();

        drawCanvas();

        game.setPixel(x, y, '^', WHITE, game.getPixelBgColor(x, y));

        game.render();
    }
}