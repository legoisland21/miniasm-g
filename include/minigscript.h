#ifndef MINIGSCRIPT_H
#define MINIGSCRIPT_H

#include <fstream>
#include <sstream>
#include <string>
#include "minigraphics.h"

using namespace std;

inline int getMinWidth(const string &path) {
    ifstream file(path);
    if (!file.is_open()) return -1;
    int w = 0, h = 0;
    file >> w >> h;
    return w;
}

inline int getMinHeight(const string &path) {
    ifstream file(path);
    if (!file.is_open()) return -1;
    int w = 0, h = 0;
    file >> w >> h;
    return h;
}

inline void loadGScript(AsciiTable &screen, const string &path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open script: " << path << endl;
        return;
    }

    int minW, minH;
    if (!(file >> minW >> minH)) {
        cerr << "Invalid header in script: " << path << endl;
        return;
    }

    if (screen.getWidth() < minW || screen.getHeight() < minH) {
        cerr << "Screen too small for script: "
             << "requires at least " << minW << "x" << minH
             << ", but screen is " << screen.getWidth() << "x" << screen.getHeight()
             << endl;
        return;
    }

    string line;
    int y = 0;
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        int val, x = 0;
        while (ss >> val) {
            char ch = '#';
            int fg = val;
            int bg = BLACK;
            screen.setPixel(x, y, ch, fg, fg);
            x++;
        }
        y++;
    }
}

#endif