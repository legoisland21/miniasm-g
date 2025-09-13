#include "include/miniasm.h"
using namespace std;

vector<Register> registers;

int main() {
    initRegisters(2);
    setRegStr("Hello, please input GSC file path: ", 0);
    setRegStr("PATH", 1);

    printStr(0, 0);
    getInputStr(1);
    setRegStr(stripQuotes(readRegStr(1)), 1);
    newLine();

    if(!fileExists(readRegStr(1))) {
        setRegStr("Could not find " + readRegStr(3), 0);
        printStr(0, 1);
        return 0;
    }

    enableANSI();
    clearWindow();
    AsciiTable screen(getMinWidth(readRegStr(1)), getMinHeight(readRegStr(1)));
    screen.clear(' ', BLACK);
    loadGScript(screen, readRegStr(1));
    screen.render();
}