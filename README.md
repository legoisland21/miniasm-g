# MiniASM-G

MiniASM-G is a tiny "assembly-style" system built in C++. You can set registers, do math, and make games!

Notice: For audio on Windows linking winmm.lib IS required
Notice 2: To have audio on Linux you need to have `alsa-utils` so to install it (apt) `sudo apt install alsa-utils` or (pacman) `sudo pacman -S alsa-utils`
Warning: If you use a slower compiler on Windows (MINGW64/TDM-GCC) it will run slower then the set FPS (use a compiler like LLVM or MSVC)
Warning 2: To enable ANSI (used for graphics) for older terminals (Windows Console Host, etc...) on Windows you need to add `enableANSI()` to enable ANSI, if the program is linux-only there is no need to add it

## How to use
1. Compile using C++11 or newer
2. Run your program
3. Have fun!

## Documentation

**Math functions (3 parameters: input1, input2, output):**  
`addReg`, `subReg`, `mulReg`, `divReg`, `modReg`, `powReg`, `incReg`, `decReg`

**Register functions:**  
- `setRegInt` / `setRegString` - sets register to value (value, index)  
- `copyRegInt` / `copyRegString` - copies register value to another (input, output)  
- `initRegisters` - initializes registers (1 parameter: number of registers)  
- `getRegisters` - returns the amount of registers  

**Input/output functions:**  
- `printInt` / `printStr` (index, newline)  
- `getInputInt` / `getInputStr` (index)  
- `readRegInt` / `readRegStr` (index)  
- `printRegisters` - prints all registers (IV = int value, SV = string value)
- `newLine` - makes a newline

**Miscellaneous**
- `randInt` (index, min, max)
- `wait` (ms)
- `enableANSI` - enables ANSI for Windows terminals (some need this)
- `clearWindow` - clear entire terminal window (uses ANSI)
- `getOS` - returns value based on OS (0 - Windows, 1 - Linux, 2 - Unknown/macOS (not supported))
- `fileExists` / `dirExists` - returns true or false based if the file/folder exists
- `stripQuotes` - strips quotes (use for anything path-related!)

**Graphics**
- `setPixel` (x, y, character, fg, bg)
- `clear` (character, bg)
- `render` - renders graphics
- `calculateDelta` (fps)
- `getWidth` / `getHeight` - gets width/height
- `getInput` / `getInputNs`  - input for games (getInputNs recommended)
- `drawText` - (x, y, text, fg, bg)
- `getPixelChar`/ `getPixelColor`/ `getPixelBgColor` - (x, y)
- `showCursor` / `hideCursor` - hides/shows cursor using ANSI

**Audio**
- `playAudio` / `playAudioNs` - plays audio (.wav only)
- `beep` / `beepns` (freq, duration) (beepns runs in another thread so no freeze)

## MiniCalc Example

```cpp
#include <iostream>
#include <vector>
#include "include/miniasm.h"
using namespace std;

vector<Register> registers;

int main() {
    initRegisters(8); // 8 registers (easier to change messages)
    setRegStr("Welcome to MiniCalc", 3);
    setRegStr("Enter operation (+, -, *, /): ", 4);
    setRegStr("Enter first number: ", 5);
    setRegStr("Enter second number", 6);
    setRegStr("Result is", 7);

    // Welcome and operation
    printStr(3, true);
    printStr(4, true);
    getInputStr(0);

    // 1st and 2nd number
    printStr(5, true);
    getInputInt(0);
    printStr(6, true);
    getInputInt(1);

    // Do operation
    if(readRegStr(0) == "+") addReg(0, 1, 2);
    else if(readRegStr(0) == "-") subReg(0, 1, 2);
    else if(readRegStr(0) == "*") mulReg(0, 1, 2);
    else if(readRegStr(0) == "/") divReg(0, 1, 2);
    else {
        setRegStr("ERROR: No valid operation", 0); // Operation not +, -, * or /
        printStr(0, true);
        return 1;
    }

    // Print result
    setRegStr("Result is " + to_string(readRegInt(2)), 7);
    printStr(7, true);
}
```

## Graphics Engine example (1 person pong)

```cpp
#include "include/miniasm.h"
#include <time.h>
using namespace std;

vector<Register> registers;
bool upD, downD = true, leftD, rightD = true;

void riff() {
    beepns(C4, 100);
    wait(100);
    beepns(E4, 100);
    wait(100);
    beepns(G4, 100);
    wait(100);
    beepns(C4, 100);
    wait(100);
    beepns(E4, 100);
    wait(100);
    beepns(G4, 100);
    wait(100);
}

int main() {
    srand(time(NULL));
    int y = rand() % 15;
    int bX = 1, bY = y + 1;
    const int fps = 30;

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
        
        bY += downD ? 1 : -1;
        bX += rightD ? 1 : -1;

        if(bY <= 0) {
            bY = 0; 
            downD = true; 
            beepns(C5, 250);
        }
        if(bY >= game.getHeight() - 1) {
            bY = game.getHeight() - 1; 
            downD = false; 
            beepns(C5, 250);
        }

        if(bX >= game.getWidth() - 1) {
            bX = game.getWidth() - 1; 
            rightD = false; 
            beepns(C5, 250);
        }

        if(bX == 1 && bY >= y-1 && bY <= y+1) {
            rightD = true;
            thread t(riff);
            t.detach();
        }
        
        char input = getInputNs();
        if(input != -1) {
            if(input == 'w' && y > 1) y--;
            if(input == 's' && y < game.getHeight() - 2) y++;
        }
        
        for(int i=-1; i<=1; i++) game.setPixel(0, y+i, '#', WHITE, WHITE);
        game.setPixel(bX, bY, 'O', BLUE, BLUE);
        
        game.render();
        wait(delta);
    }
}
```

(i know assembly looks way different but i made this for fun)