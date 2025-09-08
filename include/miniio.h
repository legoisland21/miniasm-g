#ifndef MINIIO_H
#define MINIIO_H
#include <iostream>
#include <vector>
#include "register.h"
using namespace std;

extern vector<Register> registers;

void printInt(int index, bool newline) { cout << registers[index].ivalue; if(newline) cout << endl; }
void printStr(int index, bool newline) { cout << registers[index].svalue; if(newline) cout << endl; }

void getInputInt(int index) { int result; cin >> result; registers[index].ivalue = result; }
void getInputStr(int index) { string result; getline(cin, result); registers[index].svalue = result; }

int readRegInt(int index) { return registers[index].ivalue; }
string readRegStr(int index) { return registers[index].svalue; }

void newLine() { cout << endl; }
#endif