#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<stdbool.h>
#include<ncurses.h>

#define ROWS 20
#define COLUMNS 10

int pieces[7][4][4][4] = {
    {
        {{0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0}},
        {{0, 0, 0, 0},
         {1, 1, 1, 1},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{0, 0, 1, 0},
         {0, 0, 1, 0},
         {0, 0, 1, 0},
         {0, 0, 1, 0}},
        {{0, 0, 0, 0},
         {0, 0, 0, 0},
         {1, 1, 1, 1},
         {0, 0, 0, 0}}
    },
    {
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}},
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}},
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}},
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}}
    },
    {
        {{0, 1, 0, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 0, 0, 0},
         {1, 1, 1, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {1, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}}
    },
    {
        {{1, 0, 0, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 1, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 0, 0, 0},
         {1, 1, 1, 0},
         {0, 0, 1, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {0, 1, 0, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0}}
    },
    {
        {{0, 0, 0, 0},
         {1, 1, 1, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 0, 1, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}}
    },
    {
        {{0, 0, 0, 0},
         {1, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {1, 1, 0, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0}},
        {{1, 0, 0, 0},
         {1, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 1, 0, 0},
         {1, 1, 0, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0}}
    },
    {
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0}},
        {{1, 0, 0, 0},
         {1, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}},
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0}},
        {{1, 0, 0, 0},
         {1, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 0}}
    }
};

typedef struct {
    int type; //type of piece (0-6 for the 7 tetrominoes)
    int rotation; //rotation state (0-3)
    int x; //x-axis position
    int y; //y-axis position
} Piece;

void render_board(int board[ROWS][COLUMNS], Piece current_piece) {
    //clear screen so old frames are not in the way
    erase();
    //first, draw the existing board state
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

    //then, draw the current piece on top of the board
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (pieces[current_piece.type][current_piece.rotation][py][px]) {
                int board_x = current_piece.x + px;
                int board_y = current_piece.y + py;
                if (board_x >= 0 && board_x < COLUMNS && board_y >= 0 && board_y < ROWS) {
                    mvprintw(board_y, board_x * 2, "[]");
                }
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
    nodelay(stdscr, TRUE); //make getch not block frame updates

    bool running = true;

    int board[ROWS][COLUMNS] = {0}; //initialise board array
    Piece current_piece = {0, 0, COLUMNS / 2 - 1, 0}; //initialise piece in the middle top of the board

    //placing a block
    //board[5][5] = 1;
    bool board_changed = true;

    while (running) {
        if (board_changed) {
            render_board(board, current_piece);
            board_changed = false; //reset the flag after rendering
        }
        int ch = getch();
        // Press 'q' to quit the game
        if (ch == 'q') {
            running = false;
        }
        // Example: if you modify the board elsewhere, set board_changed = true;
        // For demonstration, uncomment the next line to force a re-render every loop:
        // board_changed = true;
    }
    endwin();
    return 0;
}