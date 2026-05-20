#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
//#include <math.h>
//#include <float.h>
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
#define ROWS  3 
#define COLS 3
#define NUM_BOARD_POSITIONS 9
#define COLORED_STRING_SIZE 16
#define PLAYER_1 "Player 1"
#define PLAYER_2 "Player 2"
#define QUIT 0
#define KEEP_PLAYING 1


typedef struct {
  int row[3];
  int col[3];
  int diag_down_right;  
  int diag_up_right;  
} ThreesCount;

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

// Reset board array to positions 1 through 9.
void resetBoard(char board[][COLS], int row_size) {
  for (int i = 0; i < row_size; i++) {
    for (int j = 0; j < COLS; j++) {
      int char_code = 48 + (i * 3) + (j + 1);
      board[i][j] = (char)(char_code);
    }
  }
}

// Reset tracker for three-in-a-row count.
// X's count as +1, O's count as -1.
// If any row/column/diagonal score reaches 3 or -3, the game is over.
void resetThreesCount(ThreesCount *ptr_rcd) {
  for (int i = 0; i < 3; i++) {
    ptr_rcd->row[i] = 0;
    ptr_rcd->col[i] = 0;

  }
  ptr_rcd->diag_down_right = 0;
  ptr_rcd->diag_up_right = 0;
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

// Return player's position choice or -1 if the choice is invalid.
// Takes PLAYER_1 or PLAYER_2 constants as input.
int getPlayerChoice(char player_name[9]) {
  if (player_name == PLAYER_1){
    printf(CYAN "%s: " RESET, player_name);
  } else if (player_name == PLAYER_2) {
    printf(RED "%s: " RESET, player_name);
  } else {
    printf("Invalid player name.");
    return -1;
  }
  char choice[3];
  readLine(choice, 3);
  printf("\n");
  if (choice[0] == 'q'|| choice[0] == 'Q') {
    return INT_MIN;
  }
  if (choice[1] == '\n' && isdigit(choice[0])) {
    return choice[0] != '0' ? (choice[0] - '0'): -1;
  }
  return -1;
}

// Print instructions for game.
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

// Determines if the game has been won.
// Returns true if the game has been won, false otherwise.
bool isWinner(ThreesCount *ptr_rcd) {
  // Player 1 wins.
  for (int i = 0; i < 3; i++) { // Win on row or column.
    if (ptr_rcd->row[i] == 3 || ptr_rcd->col[i] == 3) return true;
  }
  // Win on diagonal.
  if (ptr_rcd->diag_down_right == 3 || ptr_rcd->diag_up_right == 3) return true;

  // Player 2 wins.
  for (int i = 0; i < 3; i++) { // Win on row or column.
    if (ptr_rcd->row[i] == -3 || ptr_rcd->col[i] == -3) return true;
  }
  // Win on diagonal.
  if (ptr_rcd->diag_down_right == -3 || ptr_rcd->diag_up_right==-3) return true;

  // No winner yet.
  return false;

}

// Print winning message for player.
void printWinningMessage(char winning_player[9]) {
  char *color = (winning_player == PLAYER_1) ? CYAN : RED;
  printf("Congratulations %s%s%s, you won the game!!!\n\n", 
         color,
         winning_player,
         RESET);
}

// Player can continue playing by pressing y or Y.
// Any other key entry will stop the game.
int keepPlaying() {
  char yes_or_no[2];
  printf(GREEN "Play again? " RESET "Enter Y or y for another game: ");
  readLine(yes_or_no, 2);
  printf("\n");
  if (yes_or_no[0] == 'y' || yes_or_no[0] == 'Y') return KEEP_PLAYING;
  printf(PURPLE "Thanks for playing!!!\n\n" RESET);
  return QUIT;

}

// Update the board.
// Update includes: getting player input, updating board array, 
// reprinting board,and updating row/col/diag counts.
// Returns INT_MIN if player enters q or Q. Returns 1 otherwise.
int updateBoard(char *player_name, char board[][COLS], ThreesCount *ptr_rcd) {
  while (1) {
    int playerChoice = getPlayerChoice(player_name);
    if (playerChoice == -1) {
      printf("Invalid selection. Enter a number from 1-9.\n\n");
    } else if (playerChoice == INT_MIN){
      printf(PURPLE "Thanks for playing!!!\n\n" RESET);
      return QUIT; // End of game.
    } else {
      int row_i = (playerChoice - 1) / 3;
      int col_j = (playerChoice - 1) % 3;
      char position = board[row_i][col_j];
      // Increment count if Player_1 choice, decrement if PLAYER_2 choice.
      int delta = (strcmp(player_name, PLAYER_1)) == 0 ? 1 : -1; 
      if (position == 'X' || position == 'O') {
        printf("Position already taken by %c.\n\n", position);
      }
      else {
        // Update row count.
        ptr_rcd->row[row_i] += delta;
        // PLAYER_1: increment row count.

        // Update column count.
        (ptr_rcd->col[col_j]) += delta;

        // Update diagonal count(s). (Middle position, 5, updates both diags.)
        if (row_i == col_j) {
          (ptr_rcd->diag_down_right) += delta;
          if (row_i == 1) { // Middle position in both diagonals.
            (ptr_rcd->diag_up_right) += delta;
          }
        }
        if (abs(row_i - col_j) == 2) {
          (ptr_rcd->diag_up_right) += delta;
        }
        board[row_i][col_j] = (strcmp(player_name, PLAYER_1) == 0) ? 'X' : 'O';
        printBoard(board, ROWS);
        break;
    }
  }
}  
  return KEEP_PLAYING; // Continue playing.
}

// Reset board array, board display, row/col/diag count, and turn count
void reset(char board[][COLS], int row_size, int *turn_count, ThreesCount *ptr_rcd) {
  resetBoard(board, row_size); // Reset board array.
  resetThreesCount(ptr_rcd); // Reset row/col/diag count.
  *turn_count = 0;  // Reset turn count.
}

int main(void) {
  char board[ROWS][COLS] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};
  printInstructions();
  ThreesCount rcd_count = {{0, 0, 0}, {0, 0, 0}, 0, 0}; // rcd = row_col_diag
  ThreesCount *ptr_rcd = &rcd_count;
  printBoard(board, ROWS);
  int keep_playing = KEEP_PLAYING;
  int turn_count = 0;
  int *ptr_turn_count = &turn_count;
  while(1) {
    keep_playing = updateBoard(PLAYER_1, board, ptr_rcd);
    if (!keep_playing) return QUIT;
    turn_count++;
    if (turn_count == 9 || isWinner(ptr_rcd)) {
      if (isWinner(ptr_rcd)) {
        printWinningMessage(PLAYER_1);
      } else if (turn_count == 9) {
        printf(PURPLE "It's a tie!!!\n\n" RESET);
      }
      if (keepPlaying() == QUIT) return QUIT;
      reset(board, ROWS, ptr_turn_count, ptr_rcd);
      printBoard(board, ROWS);
      continue;
    }
    keep_playing = updateBoard(PLAYER_2, board, ptr_rcd);
    if (!keep_playing) return QUIT;
    turn_count++;
    if (isWinner(ptr_rcd)) {
      printWinningMessage(PLAYER_2);
      if (keepPlaying() == QUIT) return QUIT;
      reset(board, ROWS, ptr_turn_count, ptr_rcd);
      printBoard(board, ROWS);
    }
  }
}
