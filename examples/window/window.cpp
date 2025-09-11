#include <iostream>
#include <vector>
#include "include/miniasm.h"

// crappy dos window thing

vector<Register> registers;
AsciiTable window(64, 32);

char topLeft = char(201);
char topRight = char(187);
char bottomLeft = char(200);
char bottomRight = char(188);
char horizontal = char(205);
char vertical = char(186);
char verticalRight = char(204);
char verticalLeft = char(185);
const string windowTitle = "DOS-like window";
const int FPS = window.calculateDelta(120);
string text, finalText;
string emptyText = "";
int underScoreCount = 0;
bool underScore;

void drawWindow(const string &title) {
    for(int i = 2; i != window.getWidth() - 2; i++) {
        window.setPixel(i, 1, horizontal, WHITE, BLACK);
        window.setPixel(i, window.getHeight() - 2, horizontal, WHITE, BLACK);
    }

    for(int i = 2; i != window.getHeight() - 2; i++) {
        window.setPixel(1, i, vertical, WHITE, BLACK);
        window.setPixel(window.getWidth() - 2, i, vertical, WHITE, BLACK);
    }

    window.setPixel(1, 1, topLeft, WHITE, BLACK);
    window.setPixel(1, window.getHeight() - 2, bottomLeft, WHITE, BLACK);
    window.setPixel(window.getWidth() - 2, 1, topRight, WHITE, BLACK);
    window.setPixel(window.getWidth() - 2, window.getHeight() - 2, bottomRight, WHITE, BLACK);

    window.setPixel(1, 3, verticalRight, WHITE, BLACK);
    window.setPixel(window.getWidth() - 2, 3, verticalLeft, WHITE, BLACK);
    for(int i = 2; i != window.getWidth() - 2; i++) {
        window.setPixel(i, 3, horizontal, WHITE, BLACK);
    }

    window.drawText(2, 2, title, WHITE, BLACK);
    window.setPixel(window.getWidth() - 3, 2, 'X', BLACK, RED);
}

int getWindowCornerX() {
    return 2;
}

int getWindowCornerY() {
    return 4;
}

bool inputBox(string &text, int x, int y, int width, int height, bool underscoreCount) {
    window.setPixel(x, y, topLeft, WHITE, BLACK);
    window.setPixel(x + width - 1, y, topRight, WHITE, BLACK);
    window.setPixel(x, y + height - 1, bottomLeft, WHITE, BLACK);
    window.setPixel(x + width - 1, y + height - 1, bottomRight, WHITE, BLACK);

    for(int i = x + 1; i < x + width - 1; i++) {
        window.setPixel(i, y, horizontal, WHITE, BLACK);
        window.setPixel(i, y + height - 1, horizontal, WHITE, BLACK);
    }

    for(int j = y + 1; j < y + height - 1; j++) {
        window.setPixel(x, j, vertical, WHITE, BLACK);
        window.setPixel(x + width - 1, j, vertical, WHITE, BLACK);
    }

    int input = getInputNs();
    if(input != -1) {
        if(input == I_BACKSPACE && !text.empty()) text.pop_back();
        else if(input == I_LINE_FEED || input == I_CARRIAGE_RETURN) return true;
        else text += input;
    }

    string displayText = text;
    if(underscoreCount) displayText += '_';  // add underscore cursor
    window.drawText(x + 1, y + 1, displayText, WHITE, BLACK);

    return false;
}

void textBox(string &text, int x, int y, int width, int height) {
    window.setPixel(x, y, topLeft, WHITE, BLACK);
    window.setPixel(x + width - 1, y, topRight, WHITE, BLACK);
    window.setPixel(x, y + height - 1, bottomLeft, WHITE, BLACK);
    window.setPixel(x + width - 1, y + height - 1, bottomRight, WHITE, BLACK);

    for(int i = x + 1; i < x + width - 1; i++) {
        window.setPixel(i, y, horizontal, WHITE, BLACK);
        window.setPixel(i, y + height - 1, horizontal, WHITE, BLACK);
    }

    for(int j = y + 1; j < y + height - 1; j++) {
        window.setPixel(x, j, vertical, WHITE, BLACK);
        window.setPixel(x + width - 1, j, vertical, WHITE, BLACK);
    }
    window.drawText(x + 1, y + 1, text, WHITE, BLACK);
}

int main() {
    enableANSI();
    clearWindow();
    while(true) {
        underScoreCount++;
        window.clear(' ', BLACK);
        drawWindow(windowTitle);
        bool enter = inputBox(text, getWindowCornerX() + 2, getWindowCornerY() + 1, 50, 3, underScore);
        if(enter) { finalText = text; text = emptyText; }
        textBox(finalText, getWindowCornerX() + 2, getWindowCornerY() + 4, 50, 3);
        window.render();
        wait(FPS);
        if(underScoreCount == 5) { underScore = !underScore; underScoreCount = 0; }
    }
}