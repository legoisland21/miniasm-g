# MiniASM

MiniASM is a tiny "assembly-style" system built in C++. You can set registers, do math, and probably make games!

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
- `getInput` - use for games!

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
