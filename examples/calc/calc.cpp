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