#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tictactoe.h"
#include "clear.h"
#define SIZE 3 // Grid size

char board[SIZE][SIZE];       // The game board
char currentPlayer = 'X';     // The current player ('X' or 'O')

// Initialize the board
void initializeBoard()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = ' '; // Clear each cell
        }
    }
}

// Display the board
void displayBoard()
{
    printf("\n");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf(" %c ", board[i][j]);
            if (j < SIZE - 1) printf("|"); // Column separator
        }
        printf("\n");
        if (i < SIZE - 1)
        {
            for (int j = 0; j < SIZE; j++)
            {
                printf("---");
                if (j < SIZE - 1) printf("|");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Check for a winner
int checkWinner()
{
    // Check rows
    for (int i = 0; i < SIZE; i++)
    {
        if (board[i][0] == currentPlayer &&
                board[i][1] == currentPlayer &&
                board[i][2] == currentPlayer)
            return 1;
    }

    // Check columns
    for (int i = 0; i < SIZE; i++)
    {
        if (board[0][i] == currentPlayer &&
                board[1][i] == currentPlayer &&
                board[2][i] == currentPlayer)
            return 1;
    }

    // Check diagonals
    if (board[0][0] == currentPlayer &&
            board[1][1] == currentPlayer &&
            board[2][2] == currentPlayer)
        return 1;

    if (board[0][2] == currentPlayer &&
            board[1][1] == currentPlayer &&
            board[2][0] == currentPlayer)
        return 1;

    return 0; // No winner yet
}

// Computer's move
void computerMove()
{
    int row, col;

    // Pick a random empty cell
    srand(time(0)); // Seed for random number generation
    while (1)
    {
        row = rand() % SIZE;
        col = rand() % SIZE;
        if (board[row][col] == ' ')
        {
            board[row][col] = currentPlayer;
            printf("Computer chooses row %d and column %d\n", row, col);
            break;
        }
    }
}

// The game loop
int TicTacToeX()
{
    int row, col, moves = 0;
    int isHumanTurn = 1; // 1 for player, 0 for computer

    initializeBoard();

    while (moves < SIZE * SIZE)
    {
        displayBoard();

        if (isHumanTurn)
        {
            printf("Player %c, enter row and column : ", currentPlayer);
            scanf("%d %d", &row, &col); row--;col--;

            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != ' ')
            {
                printf("Invalid move! Try again.\n");
                continue;
            }

            board[row][col] = currentPlayer;
        }
        else
        {
            computerMove(); // Computer makes its move
        }

        moves++;

        if (checkWinner())
        {
            displayBoard();
            if (isHumanTurn)
            {
                printf("Player %c wins!\n", currentPlayer);
                _sleep(2000);
                return 1; // Return 1 to indicate a win
            }
            else
            {
                printf("Computer (Player %c) wins!\n", currentPlayer);
                _sleep(2000);
                return -1;
            }
            
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X'; // Switch player
        isHumanTurn = !isHumanTurn; // Switch turn
    }

    displayBoard();
    printf("It's a draw!\n");
    return 0; // Return 0 to indicate a draw
}

int TicTacToeO() 
{
    int row, col, moves = 0;
    int isHumanTurn = 1; // 1 for player, 0 for computer
    currentPlayer = 'O';  // Set human player as 'O' from the start

    initializeBoard();

    while (moves < SIZE * SIZE)
    {
        displayBoard();

        if (isHumanTurn)
        {
            printf("Player %c, enter row and column : ", currentPlayer);
            scanf("%d %d", &row, &col); row--;col--;

            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != ' ')
            {
                printf("Invalid move! Try again.\n");
                continue;
            }

            board[row][col] = currentPlayer;
        }
        else
        {
            computerMove(); // Computer makes its move
        }

        moves++;

        if (checkWinner())
        {
            displayBoard();
            if (isHumanTurn)
            {
                printf("Player %c wins!\n", currentPlayer);
                return 1;
                _sleep(2000);
            }
            else
            {
                printf("Computer (Player %c) wins!\n", currentPlayer);
                return -1;
                _sleep(2000);
            }
            
        }

        // Switch to the other player: if it's 'O' now, it'll become 'X', and vice versa
        currentPlayer = (currentPlayer == 'O') ? 'X' : 'O'; // Switch player
        isHumanTurn = !isHumanTurn; // Switch turn
    }

    displayBoard();
    printf("It's a draw!\n");
    return 0; // Return 0 to indicate a draw
}
