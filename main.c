#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define ROWS 20
#define COLUMNS 10

int pieces[7][4][4][4] = {
    {{{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
     {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}}},
    {{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
    {{{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}},
    {{{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}},
    {{{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
     {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
    {{{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
     {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}},
    {{{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
     {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
     {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}}};

typedef struct
{
    int type;     // type of piece (0-6 for the 7 tetrominoes)
    int rotation; // rotation state (0-3)
    int x;        // x-axis position
    int y;        // y-axis position
} Piece;

int check_collision(int board[ROWS][COLUMNS], Piece piece)
{
    for (int py = 0; py < 4; py++)
    {
        for (int px = 0; px < 4; px++)
        {
            if (pieces[piece.type][piece.rotation][py][px])
            {
                int board_x = piece.x + px;
                int board_y = piece.y + py;
                // Check boundaries
                if (board_x < 0 || board_x >= COLUMNS || board_y < 0 || board_y >= ROWS)
                {
                    return 1; // Collision with wall
                }
                // Check collision with existing pieces
                if (board[board_y][board_x])
                {
                    return 1; // Collision with existing piece
                }
            }
        }
    }
    return 0; // No collision
}

void render_board(int board[ROWS][COLUMNS], Piece current_piece)
{
    // clear screen so old frames are not in the way
    erase();
    // first, draw the existing board state
    // for loop to render columns and rows, checking whether cell has a value
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLUMNS; x++)
        {
            if (board[y][x] == 0)
            {
                mvprintw(y, x * 2, " .");
            }
            else
            {
                mvprintw(y, x * 2, "[]");
            }
        }
    }

    // then, draw the current piece on top of the board
    for (int py = 0; py < 4; py++)
    {
        for (int px = 0; px < 4; px++)
        {
            if (pieces[current_piece.type][current_piece.rotation][py][px])
            {
                int board_x = current_piece.x + px;
                int board_y = current_piece.y + py;
                if (board_x >= 0 && board_x < COLUMNS && board_y >= 0 && board_y < ROWS)
                {
                    mvprintw(board_y, board_x * 2, "[]");
                }
            }
        }
    }
    refresh(); // updates the screen
}

int main(void)
{
    initscr();             // initialise ncurses
    curs_set(0);           // hides cursor
    cbreak();              // Allow user input without return by disabling line buffering
    noecho();              // don't echo input characters
    keypad(stdscr, TRUE);  // allow input with arrow keys
    nodelay(stdscr, TRUE); // make getch not block frame updates
    srand(time(NULL));     // seed random number generator

    bool running = true;

    int board[ROWS][COLUMNS] = {0}; // initialise board array
    Piece current_piece = {rand() % 7, 0, COLUMNS / 2 - 1,
                           0}; // initialise piece in the middle top of the board

    bool board_changed = true;
    time_t last_move_time = time(NULL);

    while (running)
    {
        if (board_changed)
        {
            render_board(board, current_piece);
            board_changed = false; // reset the flag after rendering
        }
        else if (current_piece.y >= 0 && check_collision(board, current_piece))
        {
            // If the piece collides immediately after moving down, it means it
            // has landed Place the piece on the board
            for (int py = 0; py < 4; py++)
            {
                for (int px = 0; px < 4; px++)
                {
                    if (pieces[current_piece.type][current_piece.rotation][py]
                              [px])
                    {
                        int board_x = current_piece.x + px;
                        int board_y = current_piece.y + py - 1; // Place piece one row above the
                                                                // collision point
                        if (board_x >= 0 && board_x < COLUMNS && board_y >= 0 && board_y < ROWS)
                        {
                            board[board_y][board_x] = 1; // Mark the cell as occupied
                        }
                    }
                }
            }
            // Spawn a new piece at the top
            current_piece.type = rand() % 7;
            current_piece.rotation = 0;
            current_piece.x = COLUMNS / 2 - 1;
            current_piece.y = 0;
            board_changed = true;
        }
        int ch = getch();
        // Press 'q' to quit the game
        if (ch == 'q')
        {
            running = false;
        }

        // Handle piece movement based on user input

        Piece attempt = current_piece; // Create a copy of the current piece to test movement
        if (ch == KEY_LEFT) attempt.x--;
        else if (ch == KEY_RIGHT) attempt.x++;
        else if (ch == KEY_DOWN) attempt.y++;
        else if (ch == KEY_UP) attempt.rotation = (attempt.rotation + 1) % 4;

        if (!check_collision(board, attempt))
        {
            current_piece = attempt; // Update the current piece if the movement is valid
            board_changed = true;
        }

        // Move piece down periodically (every second)
        time_t current_time = time(NULL);
        if (current_time - last_move_time >= 1)
        {
            current_piece.y++;
            board_changed = true;
            last_move_time = current_time;
        }
        for (int y = 0; y < ROWS; y++)
        {
            for (int x = 0; x < COLUMNS; x++)
            {
                if (board[y][x] == 0)
                {
                    mvprintw(y, x * 2, " .");
                }
                else
                {
                    mvprintw(y, x * 2, "[]");
                }
            }
        }
#ifdef _WIN32     // Windows-specific sleep function
        Sleep(1); // Sleep for 50 milliseconds to reduce CPU usage
#else             // POSIX-compliant sleep function
        usleep(1000); // Sleep for 50 milliseconds to reduce CPU usage
#endif
    }
    endwin();
    return 0;
}