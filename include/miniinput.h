#ifndef MINIINPUT_H
#define MINIINPUT_H

#define I_NULL 0
#define I_START_OF_HEADING 1
#define I_START_OF_TEXT 2
#define I_END_OF_TEXT 3
#define I_END_OF_TRANSMISSION 4
#define I_ENQUIRY 5
#define I_ACKNOWLEDGE 6
#define I_BELL 7
#define I_BACKSPACE 8
#define I_HORIZONTAL_TAB 9
#define I_LINE_FEED 10
#define I_VERTICAL_TAB 11
#define I_FORM_FEED 12
#define I_CARRIAGE_RETURN 13
#define I_SHIFT_OUT 14
#define I_SHIFT_IN 15
#define I_DATA_LINK_ESCAPE 16
#define I_DEVICE_CTRL_1 17
#define I_DEVICE_CTRL_2 18
#define I_DEVICE_CTRL_3 19
#define I_DEVICE_CTRL_4 20
#define I_NEGATIVE_ACK 21
#define I_SYNCH_IDLE 22
#define I_END_OF_BLOCK 23
#define I_CANCEL 24
#define I_END_OF_MEDIUM 25
#define I_SUBSTITUTE 26
#define I_ESCAPE 27
#define I_FILE_SEPARATOR 28
#define I_GROUP_SEPARATOR 29
#define I_RECORD_SEPARATOR 30
#define I_UNIT_SEPARATOR 31
#define I_SPACE 32
#define I_DELETE 127

#ifdef _WIN32
#include <conio.h>

int getInput() {
    return _getch();
}

int getInputNs() {
    if (_kbhit()) return _getch();
    return -1;
}

#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int getInput() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int getInputNs() {
    struct termios oldt, newt;
    int ch = -1;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return ch;
}
#endif

#endif