#ifndef MINIMISC_H
#define MINIMISC_H
#include <iostream>
#include <vector>
#include <random>
#include "register.h"
#include <chrono>
#include <thread>
using namespace std;
random_device dev;
mt19937 rng(dev());

extern vector<Register> registers;

void randInt(int index, int min, int max) { uniform_int_distribution<mt19937::result_type> random_number(min,max); registers[index].ivalue = random_number(rng); }

void wait(int ms) {
    auto start = chrono::high_resolution_clock::now();
    auto end = start + chrono::milliseconds(ms);
    while (chrono::high_resolution_clock::now() < end) {
        
    }
}

#endif