#ifndef MINIMATH_H
#define MINIMATH_H
#include <iostream>
#include <vector>
#include <math.h>
#include "register.h"
using namespace std;

extern vector<Register> registers;

void addReg(int index, int index2, int index3) { registers[index3].ivalue = registers[index].ivalue + registers[index2].ivalue; }
void subReg(int index, int index2, int index3) { registers[index3].ivalue = registers[index].ivalue - registers[index2].ivalue; }
void mulReg(int index, int index2, int index3) { registers[index3].ivalue = registers[index].ivalue * registers[index2].ivalue; }
void divReg(int index, int index2, int index3) { if (registers[index2].ivalue != 0) registers[index3].ivalue = registers[index].ivalue / registers[index2].ivalue;
else cout << "Error: divide by zero on R" << index2 << endl; }
void modReg(int index, int index2, int index3) { registers[index3].ivalue = registers[index].ivalue % registers[index2].ivalue; }
void powReg(int index, int index2, int index3) { registers[index3].ivalue = registers[index].ivalue ^ registers[index2].ivalue; }
void incReg(int index) { registers[index].ivalue++; }
void decReg(int index) { registers[index].ivalue--; }
#endif