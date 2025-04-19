#include "maze.h"
#include "clear.h"// Maze representation
char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
    {' ', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', '#'},
    {'#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', '#', '#', ' ', '#', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', 'E', '#'}};

// Player's position
int player_x = 0, player_y = 0;

// Function to print the maze
void print_maze()
{
    for (int i = 0; i < MAZE_HEIGHT; i++)
    {
        for (int j = 0; j < MAZE_WIDTH; j++)
        {
            if (i == player_x && j == player_y)
            {
                printf("P "); // Print the player
            }
            else
            {
                printf("%c ", maze[i][j]);
            }
        }
        printf("\n");
    }
}

// Function to handle player movement
int move_player(int direction)
{
    int new_x = player_x, new_y = player_y;

    switch (direction)
    {
    case UP:
        new_x -= 1;
        break;
    case DOWN:
        new_x += 1;
        break;
    case LEFT:
        new_y -= 1;
        break;
    case RIGHT:
        new_y += 1;
        break;
    }

    // Check if the new position is valid (it's a path ' ' and inside bounds)
    if (new_x >= 0 && new_x < MAZE_HEIGHT && new_y >= 0 && new_y < MAZE_WIDTH)
    {
        if (maze[new_x][new_y] != '#')
        {
            player_x = new_x;
            player_y = new_y;
        }
    }

    // Check if the player reached the exit (marked as 'E')
    if (maze[player_x][player_y] == 'E')
    {
        printf("\n\n\tYou reached the exit! You win!\n");
        _sleep(3000); // Pause for 2 seconds before exiting
        return 1;     // Game over
    }

    return 0;
}


void maze_game()
{
    int ch;
    int game_over = 0;

    while (!game_over)
    {
        clear_screen(); // Clear the screen
        print_maze();   // Print the maze

        // Get user input
        printf("Use W, A, S, D to move. Q to quit.\n");
        ch = getc(stdin); // Read the user input

        // Flush the input buffer
        while (getc(stdin) != '\n')
            ;

        switch (ch)
        {
        case 'w': // Move up
            game_over = move_player(UP);
            break;
        case 's': // Move down
            game_over = move_player(DOWN);
            break;
        case 'a': // Move left
            game_over = move_player(LEFT);
            break;
        case 'd': // Move right
            game_over = move_player(RIGHT);
            break;
        case 'q': // Quit the game
            printf("\n\n\tYou quit the game.\n");
            _sleep(3000); // Pause for 2 seconds before exiting
            game_over = 1;
            break;
        }
    }

    clear_screen(); // Clear the screen for final display
    print_maze();   // Print the final maze state
}
