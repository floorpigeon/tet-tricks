#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<stdbool.h>
#include<ncurses.h>

#define ROWS 20
#define COLUMNS 10

void render_board(int board[ROWS][COLUMNS]) {
    //clear screen so old frames are not in the way
    erase();
    //for loop to render columns and rows, checking whether cell has a value
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLUMNS; x++) {
            if (board[y][x] == 0) {
                mvprintw(y, x * 2, ". ");
            } else {
                mvprintw(y, x * 2, "[]");
            }
        }
    }
    refresh(); //updates the screen
}

int main(void) {
    //initialise ncurses
    initscr();
    int board[ROWS][COLUMNS] = {0}; //initialise board array
    render_board(board);
    endwin();
    return 0;
}