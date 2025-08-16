#ifndef DEBUG_H
#define DEBUG_H
#include <iostream>
#include <vector>
#include "register.h"
using namespace std;

extern vector<Register> registers;

void printRegisters() {
    for(const auto& r : registers) {
        cout << "R" << r.index << " IV:" << r.ivalue << " SV:" << r.svalue << endl;
    }
}
#endif