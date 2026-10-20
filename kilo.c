#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(){
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(ICRNL | IXON); //disable Ctrl-S and Ctrl-Q
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); //ICANON to read letter by letter (byte by byte) rather than line-by-line

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

//main is default starting point
int main() {
    enableRawMode();

    char c; //keyboard input into 'c' variable
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
        if (iscntrl(c)){ //is it a control ascii char?
            printf("%d\n", c);
        }else{
            printf("%d ('%c')\n", c, c);
        }
    }
    return 0; //a 0 returned indicated a success
}