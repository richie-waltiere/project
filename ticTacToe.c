#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
//#include <math.h>
//#include <stdbool.h>
//#include <float.h>
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define ROWS  3 
#define COLS 3
#define NUM_BOARD_POSITIONS 9
#define COLORED_STRING_SIZE 16
#define PLAYER_1 "Player 1: "
#define PLAYER_2 "Player 2: "

// Colors the characters on the board.
// The colored board uses an array of strings to allow for ANSI colors.
void setBoardColors(char board[][COLS], 
               int board_row_size, 
               char coloredBoard[][COLORED_STRING_SIZE], 
               int colored_board_row_size) {
  for (int i = 0; i < board_row_size; i++) {
    for (int j = 0; j < COLS; j++) {
      if (board[i][j] == 'X') {
        char player_1_color[COLORED_STRING_SIZE] = CYAN "X" RESET;
        strcpy(coloredBoard[i * COLS + j], player_1_color);
      } else if (board[i][j] == 'O'){
        char player_2_color[COLORED_STRING_SIZE] = RED "O" RESET;
        strcpy(coloredBoard[i * COLS + j], player_2_color);
      } else {
        char board_position[2] = {board[i][j],'\0'};
        char position_color[COLORED_STRING_SIZE] = YELLOW;
        strcat(position_color, board_position); 
        strcat(position_color, RESET); 
        strcpy(coloredBoard[i * COLS + j], position_color); 
      }
    }
  } 
}

// Prints the tic-tac-toe board.
void printBoard(char board[][COLS], int row_size) {
  // Colored board is an array of 9 strings.
  // Each string corresponds to one of the 9 positions on the board.
  char coloredBoard[NUM_BOARD_POSITIONS][COLORED_STRING_SIZE];
  setBoardColors(board, ROWS, coloredBoard, NUM_BOARD_POSITIONS);
  printf("\t %s | %s | %s\n"
         "\t-----------\n"
         "\t %s | %s | %s\n"
         "\t-----------\n"
         "\t %s | %s | %s\n\n",
         coloredBoard[0],
         coloredBoard[1],
         coloredBoard[2],
         coloredBoard[3],
         coloredBoard[4],
         coloredBoard[5],
         coloredBoard[6],
         coloredBoard[7],
         coloredBoard[8]
        );
}

void resetBoard(char board[][COLS], int row_size) {
  for (int i = 0; i < row_size; i++) {
    for (int j = 0; j < COLS; j++) {
      int char_code = 48 + (i * 3) + (j + 1);
      board[i][j] = (char)(char_code);
    }
  }
}

void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    // Discard characters.
  }
}

void readLine(char *ptr, int scan_size) {
  if (ptr != NULL) {
    fgets(ptr, scan_size, stdin);
    if (strchr(ptr, '\n') == NULL) {
      clearInputBuffer();
    }
  }
}

// Return position choice. Return -1 if choice is invalid.
// Use PLAYER_1 or PLAYER_2 constants as input.
int getPlayerChoice(char player_name[11]) {
  if (player_name == PLAYER_1){
    printf(CYAN "%s" RESET, player_name);
  } else if (player_name == PLAYER_2) {
    printf(RED "%s" RESET, player_name);
  } else {
    printf("Invalid player name.");
    return -1;
  }
  int player1Choice = 0;
  char choice[3];
  readLine(choice, 3);
  printf("\n");
  //printf("char 1: %d   char 2: %d\n", choice[0] - '0', choice[1]);
  if (choice[0] == 'q'|| choice[0] == 'Q') {
    return INT_MIN;
  }
  if (choice[1] == '\n' && isdigit(choice[0])) {
    return choice[0] != '0' ? (choice[0] - '0'): -1;
  }
  return -1;
}

void printInstructions(void) {
  printf(CYAN "\nWelcome to tic-tac-toe.\n\n" RESET
         "On the board below, you will see listed the numbers 1 through 9.\n"
         "Each number corresponds to a position on the board.\n"
         "Player 1 plays as %sX%s. \nPlayer 2 plays as %sO%s.\n\n",
         CYAN,  // Cyan for X.
         RESET,
         RED,   // Red for O.
         RESET);

 printf("Examples:\n"
        "If you are Player 1 and want to place an %sX%s on the board at\n" 
        "Position 3, then type 3 and press \"ENTER\".\n"
        "If you are Player 2 and want to place an %sO%s on the board at\n" 
        "Position 9, then type 9 and press \"ENTER\".\n\n"
        "First player to get three in a row wins.\n\n"
        "Let's begin.\n"
        "(Enter q at any time to quit.)\n\n",
        CYAN,
        RESET,
        RED,
        RESET
       );
}

void printWinningMessage(char winningPlayer[9]) {
  printf("Congratulations %s, you won the game!!!\n\n", winningPlayer);
}

int main(void) {
  char board[ROWS][COLS] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};
  printInstructions();
  resetBoard(board, ROWS);
  printBoard(board, ROWS);
  int row0 = 0, row1 = 0, row2 = 0;
  int col0 = 0, col1 = 0, col2 = 0;
  int diagDownRight = 0, diagUpRight = 0;  
  //printf("%d\n", getPlayerChoice(PLAYER_1));
  //printf("%d\n", getPlayerChoice(PLAYER_2));
  while(1) {
    while (1) {
      int player1Choice = getPlayerChoice(PLAYER_1);
      if (player1Choice == -1) {
        printf("Invalid selection. Enter a number from 1-9.\n\n");
      } else if (player1Choice == INT_MIN){
        printf(GREEN "Thanks for playing!!!\n\n" RESET);
        return 0;
      } else {
        char position = board[(player1Choice - 1) / 3][(player1Choice - 1) % 3];
        if (position == 'X' || position == 'O') {
          printf("Position already taken by %c.\n\n", position);
        }
        else {
          int row = (player1Choice - 1) / 3;
          int col = (player1Choice - 1) % 3;
          switch (row) {
            case 0:
              row0++;
              break;
            case 1:
              row1++;
              break;
            case 2:
              row2++;
              break;
          }
          //printf("row 0: %d row 1: %d row 2: %d\n", row0, row1, row2);
          switch (col) {
            case 0:
              col0++;
              break;
            case 1:
              col1++;
              break;
            case 2:
              col2++;
              break;
          }
          //printf("col 0: %d col 1: %d col 2: %d\n", col0, col1, col2);
          if (row == col) {
            diagDownRight++;
            if (row == 1) diagUpRight++; // Middle position in both diagonals.
          }
          if (abs(row - col) == 2) diagUpRight++;
          //printf("diagDownRight: %d diagUpRight: %d\n", diagDownRight, diagUpRight);
          board[row][col] = 'X';
          printBoard(board, ROWS);
          if (row0 == 3 || row1 == 3 || row2 == 3 ||
              col0 == 3 || col1 == 3 || col2 == 3 ||
              diagDownRight == 3 || diagUpRight == 3) {
            printWinningMessage("Player 1");
            return 0;
          }
          break;
        }
      }
    }
    while (1) {
      int player2Choice = getPlayerChoice(PLAYER_2);
      if (player2Choice == -1) {
        printf("Invalid selection. Enter a number from 1-9.\n\n");
      } else if (player2Choice == INT_MIN){
        printf(GREEN "Thanks for playing!!!\n\n" RESET);
        return 0;
      } else {
        char position = board[(player2Choice - 1) / 3][(player2Choice - 1) % 3];
        if (position == 'X' || position == 'O') {
          printf("Position already taken by %c.\n\n", position);
        }
        else {
          // check if in row; add 1 if in row if X; add -1 if O; 
          // declare winner if needed, but only after modifying board.
          int row = (player2Choice - 1) / 3;
          int col = (player2Choice - 1) % 3;
          switch (row) {
            case 0:
              row0--;
              break;
            case 1:
              row1--;
              break;
            case 2:
              row2--;
              break;
          }
          //printf("row 0: %d row 1: %d row 2: %d\n", row0, row1, row2);
          switch (col) {
            case 0:
              col0--;
              break;
            case 1:
              col1--;
              break;
            case 2:
              col2--;
              break;
          }
          //printf("col 0: %d col 1: %d col 2: %d\n", col0, col1, col2);
          if (row == col) {
            diagDownRight--;
            if (row == 1) diagUpRight--; // Middle position in both diagonals.
          }
          if (abs(row - col) == 2) diagUpRight--;
          //printf("diagDownRight: %d diagUpRight: %d\n", diagDownRight, diagUpRight);
          board[row][col] = 'O';
          printBoard(board, ROWS);
          if (row0 == -3 || row1 == -3 || row2 == -3 ||
              col0 == -3 || col1 == -3 || col2 == -3 ||
              diagDownRight == -3 || diagUpRight == -3) {
            printWinningMessage("Player 2");
            return 0;
          }
          break;
        }
      }
    }
  }
  return 0;
}
