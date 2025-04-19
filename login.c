#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h" // Adding the header file
#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

int registerUser()   // function decleared int the header
{
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    FILE *file = fopen("users.txt", "a");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 0;
    }

    printf("Enter a username: ");
    scanf("%s", username);

    printf("Enter a password: ");
    scanf("%s", password);

    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    printf("Registration successful!\n");
    return 1;
}

int loginUser()   // function decleared int the header
{
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    char fileUsername[MAX_USERNAME], filePassword[MAX_PASSWORD];
    FILE *file = fopen("users.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 0;
    }

    printf("Enter your username: ");
    scanf("%s", username);

    printf("Enter your password: ");
    scanf("%s", password);

    while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF)
    {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0)
        {
            fclose(file);
            return 1; // succesful login
        }
    }

    fclose(file);
    printf("Wrong username or password");
    sleep_ms(2000);
    return 0; // failed log in
}