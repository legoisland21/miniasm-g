# MiniASM

MiniASM is a tiny "assembly-style" system built in C++. You can set registers, do math, and make games!

Notice: For audio on Windows including winmm.lib IS required
Warning: Linux runs way faster then Windows, while making pong 5FPS on linux was about the same as 30FPS on windows

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

**Miscellaneous**
- `randInt` (index, min, max)
- `wait` (ms)
- `beep` (freq, duration)
- `beepns` (freq, duration) (runs in another thread so no freeze)

**Graphics**
- `setPixel` (x, y, character, fg, bg)
- `clear` (character, bg)
- `render` - renders graphics
- `calculateDelta` (fps)
- `getWidth` / `getHeight` - gets width/height
- `getInput` - used mainly for games.
- `getInputns` - better over getInput as it skips when no input is provided.
- `drawText` - (x, y, text, fg, bg)
- `getPixelChar`/`getPixelColor`/`getPixelBgColor` - (x, y)

## MiniCalc Example

```cpp
#include <iostream>
#include <vector>
#include "include/miniasm.h"
using namespace std;

vector<Register> registers;

int main() {
    initRegisters(4); // Initializes 4 registers
    setRegStr("Welcome to MiniCalc!", 0);
    printStr(0, true); // Sets and outputs register value

    setRegStr("Choose operation (1 = add, 2 = sub, 3 = mul, 4 = div): ", 0);
    printStr(0, false); // Outputs register value
    getInputInt(4); // Gets int from user and sets to register 4
    if(readRegInt(4) < 1 || readRegInt(4) > 4) {
        setRegStr("Invalid operation", 0);
        printStr(0, false);
        return 0;
    }

    setRegStr("Choose 1st number: ", 0);
    printStr(0, false); 
    getInputInt(1); 

    setRegStr("Choose 2nd number: ", 0);
    printStr(0, false); 
    getInputInt(2); 

    if(readRegInt(4) == 1) addReg(1, 2, 3); 
    else if(readRegInt(4) == 2) subReg(1, 2, 3); 
    else if(readRegInt(4) == 3) mulReg(1, 2, 3); 
    else if(readRegInt(4) == 4) divReg(1, 2, 3);

    setRegStr("Result is: ", 0);
    printStr(0, false); 
    printInt(3, false); 

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