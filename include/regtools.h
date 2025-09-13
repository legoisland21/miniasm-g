#ifndef REGTOOLS_H
#define REGTOOLS_H
#include <iostream>
#include <vector>
#include "register.h"
using namespace std;

extern vector<Register> registers;

int regs_global = 0;

void setRegInt(int value, int index) { registers[index].ivalue = value; }
void copyRegInt(int index, int index2) { registers[index].ivalue = registers[index2].ivalue; }

void setRegStr(string value, int index) { registers[index].svalue = value; }
void copyRegStr(int index, int index2) { registers[index].svalue = registers[index2].svalue; }

void initRegisters(int regs = 16) {
    regs_global = regs;
    registers.clear();
    for(int i = 0; i != regs; i++) {
        registers.push_back({i, 0, ""});
    }
}

void clearRegisters() {
    for(Register reg : registers) {
        reg.ivalue = 0;
        reg.svalue = "";
    }
}

int getRegisters() { return regs_global; }

#endif