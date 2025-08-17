#ifndef MINIGRAPHICS_H
#define MINIGRAPHICS_H
#include <vector>
#include <iostream>
#include "minicolors.h"
using namespace std;

struct Pixel {
    char ch;
    int fg;
    int bg;
    Pixel(char c = ' ', int f = WHITE, int b = BLACK) : ch(c), fg(f), bg(b) {}
};

class AsciiTable {
private:
    int width, height;
    vector<vector<Pixel>> table;

    string ansiColor(int fg, int bg) {
        return "\033[3" + to_string(fg) + ";4" + to_string(bg) + "m";
    }

public:
    AsciiTable(int w, int h) : width(w), height(h), table(h, vector<Pixel>(w)) {}

    void setPixel(int x, int y, char ch, int fg = WHITE, int bg = BLACK) {
        if (x >= 0 && x < width && y >= 0 && y < height)
            table[y][x] = Pixel(ch, fg, bg);
    }

    void clear(char c = ' ', int bg = BLACK) {
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                table[y][x] = {c, WHITE, bg};
            }
        }
    }

    void render() {
        cout << "\033[H";
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel &p = table[y][x];
                cout << ansiColor(p.fg, p.bg) << p.ch;
            }
            cout << "\033[0m\n";
        }
        cout.flush();
    }

    int getWidth() { return width; }
    int getHeight() { return height; }

    int calculateDelta(int fps) {
        if (fps <= 0) return 0;
        return static_cast<int>(1000.0 / fps);
    }
};

#endif