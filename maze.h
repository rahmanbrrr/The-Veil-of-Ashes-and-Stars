#ifndef MAZE_GAME_H
#define MAZE_GAME_H

#include <stdio.h>
#include <stdlib.h>

#define MAZE_HEIGHT 10
#define MAZE_WIDTH 10

// Directions for moving the player
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// External maze and player position
extern char maze[MAZE_HEIGHT][MAZE_WIDTH];
extern int player_x, player_y;

// Function declarations
void print_maze();
int move_player(int direction);
void maze_game();

#endif // MAZE_GAME_H
