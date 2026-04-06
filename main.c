#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<stdbool.h>
#include<ncurses.h>

#define ROWS 20
#define COLUMNS 10

typedef struct {
    int type; //type of piece (0-6 for the 7 tetrominoes)
    int rotation; //rotation state (0-3)
    int x; //x-axis position
    int y; //y-axis position
} Piece;

void render_board(int board[ROWS][COLUMNS]) {
    //clear screen so old frames are not in the way
    erase();
    //for loop to render columns and rows, checking whether cell has a value
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLUMNS; x++) {
            if (board[y][x] == 0) {
                mvprintw(y, x * 2, " .");
            } else {
                mvprintw(y, x * 2, "[]");
            }
        }
    }
    refresh(); //updates the screen
}

int main(void) {
    initscr(); //initialise ncurses
    curs_set(0); //hides cursor
    cbreak(); //Allow user input without return by disabling line buffering
    noecho(); //don't echo input characters
    keypad(stdscr, TRUE); //allow input with arrow keys

    bool running = true;

    int board[ROWS][COLUMNS] = {0}; //initialise board array

    //placing a block
    //board[5][5] = 1;
    while (running) {
        render_board(board);
        getch(); //not 100% sure what this does but used for now to pause for a second
    }
    endwin();
    return 0;
}