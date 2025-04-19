#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "combat.h"
#include "maze.h"
#include "login.h"
#include "tictactoe.h"
#include "clear.h"
#ifdef _WIN32
#include <windows.h>//for windows
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>//for linux/macos
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

// Game state tracking variables for save/load functionality
int gameProgress = 0;    // Tracks overall progress through the story
int characterPath = 0;   // 1=Scholar, 2=Knight, 3=Scientist
int currentLocation = 0; // Tracks current location/scene
int hasWatchersKey = 0;
int hasObaidulMask = 0;
int hasCelestialCompass = 0;

int hasSigilOfClarity = 0;
int hasStrengthBonus = 0;
int hasPuzzleLens = 0;
int hasTarekCharm = 0;
int hasWeapon = 0;

// Function prototypes for game scenes
void displayPrologue();
void displayChapter1();
void displayChapter2();
void displayLevel1();
void displayLevel2();
void displayLevel3();
void displayLevel4();
void displayLevel5();
void displayEpilogue();

// Function prototypes for game mechanics
void saveGame();
void loadGame();
void displayMap();
void displayMenu();
void gameOver();

// Helper function to get and process player choice
char getPlayerChoice()
{
    char choice;
    int valid = 0;

    while (!valid)
    {
        printf("\nEnter your choice (m for map, q to quit, h for menu): ");
        scanf(" %c", &choice);
        choice = tolower(choice);

        // Process menu commands
        switch (choice)
        {
        case 'm': // Map
            displayMap();
            break;
        case 'q': // Quit
            printf("\nSaving game progress...\n");
            saveGame();
            printf("Thank you for playing The Veil of Ash & Stars. Goodbye!\n");
            sleep_ms(2000); // Pause for 2 seconds
            exit(0);    // Exit the game
            return 'q'; // Never reached, but added for clarity
        case 'h':       // Help menu
            displayMenu();
            break;
        default:
            // Return any other valid choice to the calling function
            valid = 1;
            break;
        }
    }

    return choice;
}

// Save game progress to file
void saveGame()
{
    FILE *saveFile = fopen("savegame.txt", "w");
    if (saveFile == NULL)
    {
        printf("Error: Unable to create save file.\n");
        return;
    }

    // Write game state variables to file
    fprintf(saveFile, "%d\n%d\n%d\n%d\n%d\n%d\n",
            gameProgress, characterPath, currentLocation,
            hasWatchersKey, hasObaidulMask, hasCelestialCompass);

    fclose(saveFile);
    printf("Game saved successfully!\n");
}

// Load game progress from file
void loadGame()
{
    FILE *saveFile = fopen("savegame.txt", "r");
    if (saveFile == NULL)
    {
        printf("Error: No save file found.\n");
        return;
    }

    // Read game state variables from file
    fscanf(saveFile, "%d\n%d\n%d\n%d\n%d\n%d\n",
           &gameProgress, &characterPath, &currentLocation,
           &hasWatchersKey, &hasObaidulMask, &hasCelestialCompass);

    fclose(saveFile);
    printf("Game loaded successfully!\n");

    // Resume game from saved point
    switch (currentLocation)
    {
    case 0:
        displayPrologue();
        clear_screen();
        break;
    case 1:
        displayChapter1();
        clear_screen();
        break;
    case 2:
        displayChapter2();
        clear_screen();
        break;
    case 3:
        displayLevel1();
        clear_screen();
        break;
    case 4:
        displayLevel2();
        clear_screen();
        break;
    case 5:
        displayLevel3();
        clear_screen();
        break;
    case 6:
        displayLevel4();
        clear_screen();
        break;
    case 7:
        displayLevel5();
        clear_screen();
        break;
    case 8:
        displayEpilogue();
        break;
    default:
        displayPrologue();
        break;
    }
}

// Display game map
void displayMap()
{
    printf("\n==== THE VEIL OF ASH & STARS - MAP ====\n");
    printf("Current progress: ");

    switch (currentLocation)
    {
    case 0:
        printf("Prologue - The Forgotten Prophecy\n");
        break;
    case 1:
        printf("Chapter 1 - The Choice of Fates\n");
        break;
    case 2:
        printf("Chapter 2 - Journey to the Isle of Echoes\n");
        break;
    case 3:
        printf("Level 1 - Echoes of the Infinite Grid\n");
        break;
    case 4:
        printf("Level 2 - The Riddle of the Watchers\n");
        break;
    case 5:
        printf("Level 3 - The Priest's Maze of Light and Shadow\n");
        break;
    case 6:
        printf("Level 4 - The Right Hand Man's Ambush\n");
        break;
    case 7:
        printf("Level 5 - The Final Combat\n");
        break;
    case 8:
        printf("Epilogue - The Veil's Judgement\n");
        break;
    default:
        printf("Unknown\n");
        break;
    }

    printf("\nYour character: ");
    switch (characterPath)
    {
    case 1:
        printf("Scholar\n");
        break;
    case 2:
        printf("Knight\n");
        break;
    case 3:
        printf("Scientist\n");
        break;
    default:
        printf("Mystery\n");
        break;
    }

    printf("\nItems collected:\n");
    if (hasWatchersKey)
        printf("- The Watcher's Key\n");
    if (hasObaidulMask)
        printf("- Obaidul's Mask\n");
    if (hasCelestialCompass)
        printf("- Celestial Compass\n");

    printf("\n======================================\n");
}

// Display help menu
void displayMenu()
{
    printf("\n==== THE VEIL OF ASH & STARS - MENU ====\n");
    printf("m - Display Map\n");
    printf("q - Save and Quit Game\n");
    printf("h - Display this Menu\n");
    printf("\nDuring choices, enter the letter or number\n");
    printf("corresponding to your desired action.\n");
    printf("=====================================\n");
}

// Game over screen with restart option
void gameOver()
{
    char choice;
    printf("\n============ GAME OVER ============\n");
    printf("\nDO YOU WANT TO RESTART?\n");
    printf("1. YES\n");
    printf("2. NO\n");

    while (1)
    {
        choice = getPlayerChoice();

        if (choice == '1')
        {
            // Reset game state
            gameProgress = 0;
            characterPath = 0;
            currentLocation = 0;
            hasWatchersKey = 0;
            hasObaidulMask = 0;
            hasCelestialCompass = 0;

            displayPrologue();
            return;
        }
        else if (choice == '2')
        {
            printf("\nThank you for playing The Veil of Ash & Stars. Goodbye!\n");
            sleep_ms(2000); // Pause for 2 seconds
            exit(0);
        }
        else
        {
            printf("Invalid choice. Please enter 1 or 2.\n");
        }
    }
}

// Main game content functions - left blank as requested
void displayPrologue()
{
    currentLocation = 0;

    // Display prologue content here
    printf("\n\nPrologue: The Forgotten Prophecy\n\n");
    printf("In the ancient ages, the world was guarded by The Watchers, the heavenly keepers who maintained equilibrium among the worlds. The Watchers vanished but left only a cryptic prophecy:\n\n");
    printf("   \"When the Veil of Ash and Stars is torn\n");
    printf("   One shall rise; choose your truth,\n");
    printf("   Yet death awaits where the Shadow stands.\"\n\n");
    printf("Now, the veil weakens, and the Evil stirs. You must uncover the truth before reality itself dissolves.\n\n");
    printf("(The screen fades in black. A voice whispered in layered echoes.)\n\n");
    printf("A cold breeze stirs the ancient banners in a forgotten land. You find yourself lying on polished stone under a cracked ceiling with stars bleeding through. You're in The Brownstown Hall, where it time and reality bends.\n\n");

    // Continue to Chapter 1
    gameProgress = 1;
    displayChapter1();
}

void displayChapter1()
{
    currentLocation = 1;
    char choice;
    // Display Chapter 1 content here
    printf("\n\nChapter 1: The Choice of fates\n");
    printf("------------------------------\n\n");
    printf("Place: Brownstown Hall\n\n");
    printf("Ancient, dreamlike filled with glowing constellations suspended in the air.\n\n");
    printf("Voices of the hall (softly whispering) \"Do you remember fall? The moment the stars went dark?\"\n\n");
    printf("(A vision flashes shattered skies, a city of black glass, a robed figure with no face.)\n\n");
    printf("Voices of hall (louder) \"The Veil is breaking. You are the last. Choose wisely.\"\n\n");
    printf("(You wake up cold stone beneath you. The air smells of burnt parchment and iron.)\n\n");
    printf("Voices of hall (calmly) \"Four paths lie before you, Seeker. One leads to ruin. One to glory. One to truth. And one… to silence. Will you light the path... or be devoured by it?\"\n\n");
    printf("(Four archways glow each pulsing with eerie light.)\n\n");
    printf("An ancient presence offers you four life paths, each shaping your destiny:\n\n");

    printf("1. The Scholar's Path: Seek wisdom in forgotten tomes, intuitive and skilled\n");
    printf("2. The Knight's Path: Rely on courage and blade, born for direct combat\n");
    printf("3. The Scientist's Path: Technologically advanced, trust in logic and invention\n");
    printf("4. The Whispering Path: A voice leads you...somewhere unknown\n");

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case '1':
            printf("\nYou choose: \"I seek knowledge.\"\n\n");
            printf("(The air hums. Pages of unseen books flutter around you.)\n\n");
            printf("Voices of hall- \"Will you be able to save them?\"\n\n");
            printf("\"Or will you join them in the end?\"\n\n");
            characterPath = 1; // Scholar
            gameProgress = 2;
            displayChapter2();
            return;

        case '2':
            printf("\nYou choose: \"With courage and strength, I trust my blade than anyone else.\"\n\n");
            printf("(Your hand twitches toward a sword you don't have. The taste of blood fills your mouth.)\n\n");
            printf("Voices of hall- \"The last knight of a dead order. How… fitting.\"\n\n");
            characterPath = 2; // Knight
            gameProgress = 2;
            displayChapter2();
            return;

        case '3':
            printf("\nYou choose: \"With the help of my logic and invention, I can fix the destiny.\"\n\n");
            printf("(A mechanical whirring—like gears grinding backward. Your vision glitches.)\n\n");
            printf("Voices of hall- \"Even stars obey laws. Will you dare to break them?\"\n\n");
            characterPath = 3; // Scientist
            gameProgress = 2;
            displayChapter2();
            return;

        case '4':
            printf("\nYou choose: \"I can hear another voice!\"\n\n");
            printf("Unknown voice- \"Come with me.\"\n\n");
            printf("(The ground crumbles beneath you and snatches you to darkness)\n\n");
            printf("GAME OVER!\n\n");
            gameOver();
            return;

        default:
            printf("Please choose a valid option (1-4).\n");
            break;
        }
    }
}

void displayChapter2()
{
    // Implement Chapter 2 content
    currentLocation = 2;
    char choice;
    printf("\n\nChapter 2: Journey to the Isle of Echoes\n");
    printf("----------------------------------------\n\n");

    printf("Survivors must reach the Isle of Echoes, where the trial begins.\n\n");
    printf("(The world rumbles. The veil trembles. A map is presented to you, offering three ways to reach the sacred Isle of Echoes.)\n\n");
    printf("Your route choices are:\n\n");
    printf("1. Flying Sky ship\n");
    printf("2. Abandoned boat\n");
    printf("3. Old scattered road\n");

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case '1':
            printf("\nYou choose: Flying Sky ship\n\n");
            printf("(The ship groans as lightning strikes its wings.)\n\n");
            printf("One of the crew-man - \"They say the storm eats souls.\"\n\n");
            printf("You: \"I don't think it's real. What make them think like that?\"\n\n");
            printf("Crew-man(nervous) \"They believe that it's not a storm. It's alive.\"\n\n");
            printf("A bolt of lightning strikes the deck. A ghost appears briefly at the helm.\n\n");
            printf("Ghostly Figure: \"Don't trust the one with eyes like fire. He sees beyond the veil.\"\n\n");
            sleep_ms(5000); // Pause for 2 seconds
            gameProgress = 3;
            displayLevel1();
            return;

        case '2':
            printf("\nYou choose: Abandoned boat\n\n");
            printf("(The sea is black, moving unnaturally. Something scrapes the hull.)\n\n");
            printf("(A face exactly like yours, stares back before dissolving.)\n\n");
            sleep_ms(3000);
            gameProgress = 3;
            displayLevel1();
            return;

        case '3':
            printf("\nYou choose: Old scattered road\n\n");
            printf("(The road is lined with statues. Each a failed Harbinger.)\n\n");
            printf("You've found a Celestial Compass! This will help you navigate mazes later.\n\n");
            sleep_ms(3000); // Pause for 2 seconds
            hasCelestialCompass = 1;
            gameProgress = 3;
            displayLevel1();
            return;

        default:
            printf("Please choose a valid option (1-3).\n");
            break;
        }
    }
}

void displayLevel1()
{
    currentLocation = 3;
    char choice;

    // Level 1 introduction
    clear_screen();
    printf("\n\nLevel 1: Echoes of the Infinite Grid\n");
    printf("-------------------------------------\n\n");
    printf("You arrived on the isle after a huge tragic journey.\n");
    printf("The Isle of Echoes shimmers with strange auroras. Ashen sand stretches into a fractured sky.\n");
    printf("The ancient grid rises—a glowing, celestial Tic-Tac-Toe board floating above a mist-laced lake.\n\n");
    printf("On the Isle, the Seeker must face a living game board, a cosmic Tic-Tac-Toe arena haunted by echoes of failed warriors.\n\n");
    printf("As soon as you step onto the game platform the air crackles with energy. The stone golem's empty eyes track your every movement.\n\n");
    printf("Twist: Each cell contains memory fragment. Winning unlocks a memory of a forgotten war; losing erases your memories.\n\n");
    printf("GOLEM: \"Seeker! You have stepped upon the grid of remembrance. Win, and you shall see what was hidden. Lose and forget who you are.\"\n\n");

    printf("Choose your response:\n");
    printf("a. \"Who are you? What is this place?\"\n");
    printf("b. \"I'm not afraid. Let's begin.\"\n");
    printf("c. \"This is just a game... right?\"\n");

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case 'a':
            printf("\nYou ask: \"Who are you? What is this place?\"\n\n");
            printf("GOLEM: \"Games are but rituals masked in innocence.\"\n\n");
            break;
        case 'b':
            printf("\nYou say: \"I'm not afraid. Let's begin.\"\n\n");
            printf("GOLEM: \"Games are but rituals masked in innocence.\"\n\n");
            break;
        case 'c':
            printf("\nYou hesitantly ask: \"This is just a game... right?\"\n\n");
            printf("GOLEM: \"Games are but rituals masked in innocence.\"\n\n");
            break;
        default:
            printf("Please choose a valid option (a, b, or c).\n");
            continue;
        }
        break;
    }

    printf("GOLEM: \"X or O, Seeker? Choose quickly - the board hungers.\"\n\n");
    printf("Choose your symbol:\n");
    printf("a. \"I'll be X\"\n");
    printf("b. \"I'll be O\"\n");
    printf("c. \"What happens if I refuse?\"\n");

    int gameWon;

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case 'a':
            printf("\nYou choose: \"I'll be X\"\n");
            printf("(The symbol burns itself into your palm)\n\n");
            gameWon = TicTacToeX();
            break;
        case 'b':
            printf("\nYou choose: \"I'll be O\"\n");
            printf("(Your breath frosts in the air)\n\n");
            gameWon = TicTacToeO();
            break;
        case 'c':
            printf("\nYou ask: \"What happens if I refuse?\"\n");
            printf("(The golem's smile cracks wider)\n\n");
            printf("GOLEM: \"Then you forget... your past.\"\n");
            printf("(A memory is ripped from your mind - you suddenly can't remember your mother's face.)\n\n");
            printf("You lose. Try again? (y/n)\n");

            while (1)
            {
                choice = getPlayerChoice();

                if (choice == 'y')
                {
                    printf("\nYou decide to try again.\n\n");
                    // Return to symbol selection
                    displayLevel1();
                    return;
                }
                else if (choice == 'n')
                {
                    printf("\nYou choose to give up.\n");
                    printf("GAME OVER\n");
                    sleep_ms(2000);
                    exit(0);
                }
                else
                {
                    printf("Please enter y or n.\n");
                }
            }
            break;
        default:
            printf("Please choose a valid option (a, b, or c).\n");
            continue;
        }
        break;
    }

    if (gameWon)
    {
        printf("\n(The golem's stone face cracks open, revealing a screaming void)\n");
        printf("GOLEM: Smart move! But cleverness killed the Watchers too.\n");
        printf("\"Take your reward. But know this—every step closer brings you nearer the edge.\"\n\n");

        // Award path-specific rewards
        if (characterPath == 1)
        { // Scholar
            printf("You receive the Sigil of Clarity, which reveals hidden clues.\n");
            hasSigilOfClarity = 1;
        }
        else if (characterPath == 2)
        { // Knight
            printf("Your strength increases by +7!\n");
            hasStrengthBonus = 1;
        }
        else if (characterPath == 3)
        { // Scientist
            printf("You unlock the Puzzle Lens, allowing you to see hidden patterns.\n");
            hasPuzzleLens = 1;
        }
    }
    else
    {
        printf("\n(Your vision blurs. When it finally clears, your character sheet shows -1 to all stats)\n");
        printf("GOLEM: \"How much of yourself can you afford to lose before you become nobody?\"\n\n");
    }

    printf("\nWant to start Level 2? (y/n)\n");
    while (1)
    {
        choice = getPlayerChoice();

        if (choice == 'y')
        {
            gameProgress = 4;
            displayLevel2();
            return;
        }
        else if (choice == 'n')
        {
            printf("\nThank you for playing The Veil of Ash & Stars. Goodbye!\n");
            exit(0);
        }
        else
        {
            printf("Please enter y or n.\n");
        }
    }
}

void displayLevel2()
{
    currentLocation = 4;
    char choice;
    char answer[20];
    char correctAnswer[20] = "sugarcane";
    clear_screen();
    printf("\n\nLevel 2: The Riddle of the Watchers\n");
    printf("------------------------------------\n\n");
    printf("After crossing the dense island, you enter a sacred cave where glowing murals on the wall are your given riddles.\n");
    printf("The air pulses with arcane energy, riddles carved in celestial glyphs.\n\n");
    printf("(The cave walls breathe. The three faces - Truth, Lies, and Chaos. Keeps changing positions when you blink.)\n\n");
    printf("MURAL: \"Ask me anything! I'll answer. MAYBE!\"\n");
    printf("(Giggles madly)\n\n");

    printf("Choose your question:\n");
    printf("a. \"Which path leads to safety?\"\n");
    printf("b. \"What is the real question I should ask?\"\n");
    printf("c. \"What's the meaning of life?\"\n");

    // Show Scholar-specific option if character is on Scholar path and has the Sigil
    if (characterPath == 1 && hasSigilOfClarity)
    {
        printf("d. Use Sigil of Clarity\n");
    }

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case 'a':
            printf("\nYou ask: \"Which path leads to safety?\"\n");
            printf("(MURAL points in all directions at once.)\n");
            printf("You are confused by the contradictory guidance.\n\n");
            printf("[Unknown Voice]: \"Many failed here, not because they were wrong, but because they doubted themselves.\"\n\n");
            printf("Try again? (y/n)\n");
            while (1)
            {
                choice = getPlayerChoice();

                if (choice == 'y')
                {
                    printf("\nYou decide to try again.\n\n");
                    displayLevel2();
                    return;
                }
                else if (choice == 'n')
                {
                    printf("\nYou choose to give up.\n");
                    printf("GAME OVER\n");
                    exit(0);
                }
                else
                {
                    printf("Please enter y or n.\n");
                }
            }
            sleep_ms(3000);
            // Return to question selection
            displayLevel2();
            return;

        case 'b':
            printf("\nYou ask: \"What is the real question I should ask?\"\n");
            printf("(Truth and lie both start bleeding from their eyes.)\n");
            printf("You have found the right path forward!\n\n");
            break;

        case 'c':
            if (characterPath == 1 && hasSigilOfClarity)
            {
                printf("\nYou use the Sigil of Clarity...\n");
                printf("(The walls whisper forbidden knowledge)\n");
                printf("Your sigil tingles. A vision flashes - Lies always touches Truth when they think no one's looking.\n\n");
                printf("This insight helps you find the right path!\n\n");
                break;
            }
            else
            {
                printf("Please choose a valid option.\n");
                continue;
            }

        case 'd':
            printf("\nYou ask: \"What's the meaning of life?\"\n");
            printf("MURAL: \"42! Wait... no... death! No... cheese? Ask again!\"\n");
            printf("(The cave starts collapsing from the nonsense)\n\n");
            printf("[Unknown Voice]: \"Many failed here, not because they were wrong, but because they doubted themselves.\"\n\n");
            printf("Try again? (y/n)\n");

            while (1)
            {
                choice = getPlayerChoice();

                if (choice == 'y')
                {
                    printf("\nYou decide to try again.\n\n");
                    displayLevel2();
                    return;
                }
                else if (choice == 'n')
                {
                    printf("\nYou choose to give up.\n");
                    printf("GAME OVER\n");
                    exit(0);
                }
                else
                {
                    printf("Please enter y or n.\n");
                }
            }
            break;

        default:
            printf("Please choose a valid option.\n");
            continue;
        }
        break;
    }

    // Second riddle after passing the first one
    printf("You've passed the first test. A new riddle appears on the wall:\n\n");
    printf("\"Allah r ki kudrot, lathir bhitor shorbot.\"\n\n");
    printf("What is it? (answer in english, lowercase)\n> ");

    scanf("%19s", answer);

    if (strcmp(answer, correctAnswer) == 0)
    {
        printf("\nCorrect answer. You pass.\n\n");
        hasWatchersKey = 1;
        printf("You have obtained the Watcher's Key that unlocks the Priest's sanctuary!\n\n");
        printf("[Unknown Voice]: \"The Watcher's Key is now yours. But beware—keys don't just open doors. They lock them too.\"\n\n");
    }
    else
    {
        printf("\nWrong answer. The cave trembles.\n\n");
        printf("Try again? (y/n)\n");

        while (1)
        {
            choice = getPlayerChoice();

            if (choice == 'y')
            {
                printf("\nYou decide to try again.\n\n");
                displayLevel2();
                return;
            }
            else if (choice == 'n')
            {
                printf("\nYou choose to give up.\n");
                printf("GAME OVER\n");
                exit(0);
            }
            else
            {
                printf("Please enter y or n.\n");
            }
        }
    }

    printf("\nWant to start Level 3? (y/n)\n");
    while (1)
    {
        choice = getPlayerChoice();

        if (choice == 'y')
        {
            gameProgress = 5;
            displayLevel3();
            return;
        }
        else if (choice == 'n')
        {
            printf("\nThank you for playing The Veil of Ash & Stars. Goodbye!\n");
            exit(0);
        }
        else
        {
            printf("Please enter y or n.\n");
        }
    }
}

void displayLevel3()
{
    currentLocation = 5;
    char choice;
    clear_screen();

    printf("\nLevel 3: The Priest's Maze of Light and Shadow\n");
    printf("----------------------------------------------\n\n");
    printf("PLACE: A labyrinth carved from starstone, where shifting light beams guide or deceive.\n");
    printf("The walls hum with forgotten hymns.\n\n");
    printf("You meet Priest TAREK.\n\n");
    printf("(Tarek's chapel smells of incense and rotting meat. His hands shake as he pours you juice.)\n\n");
    printf("PRIEST TAREK: \"Drink, Seeker. You'll need strength for what comes. You've done well.\n");
    printf("Let me help you reach the sanctum. This maze is alive, a beast of shadow and memory.\n");
    printf("Take this charm. It will protect you… for now.\"\n\n");

    printf("Choose your response:\n");
    printf("a. \"Why are you helping me?\"\n");
    printf("b. \"What lies beyond the maze?\"\n");
    printf("c. \"I don't trust gifts so easily.\"\n");

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case 'a':
            printf("\nYou ask: \"Why are you helping me?\"\n\n");
            break;
        case 'b':
            printf("\nYou ask: \"What lies beyond the maze?\"\n\n");
            break;
        case 'c':
            printf("\nYou say: \"I don't trust gifts so easily.\"\n\n");
            break;
        default:
            printf("Please choose a valid option (a, b, or c).\n");
            continue;
        }
        break;
    }

    printf("PRIEST TAREK: \"Because your fate is woven with mine. And because I was once lost, too.\n");
    printf("What lies ahead? The truth. But truth comes with cost.\"\n\n");

    printf("You enter the maze. There are three paths before you:\n");
    printf("a. Left path: \"The walls whisper your childhood secrets\"\n");
    printf("b. Right path: \"You hear your own voice begging you to turn back\"\n");
    printf("c. Straight ahead: \"Your shadow walks ahead of you... and turns to look back\"\n");

    while (1)
    {
        choice = getPlayerChoice();

        if (choice == 'a' || choice == 'b' || choice == 'c')
        {
            maze_game(); // maze game function
            printf("\nYou navigate through the treacherous maze, following the cryptic guidance of Tarek's words.\n");
            printf("After what seems like hours, you finally reach the end of the labyrinth.\n\n");
            break;
        }
        else
        {
            printf("Please choose a valid option (a, b, or c).\n");
        }
    }

    printf("PRIEST TAREK: \"You are stronger now. Take your weapon, chosen one.\n");
    printf("You will need it where shadows dare not tread.\"\n\n");

    // Award path-specific weapons
    if (characterPath == 1)
    { // Scholar
        printf("You receive the Tome of Eldritch Words and Sight, capable of weakening enemies.\n");
        hasWeapon = 1;
    }
    else if (characterPath == 2)
    { // Knight
        printf("You receive the Sword of the Ash, which has a chance to defeat enemies in a single blow.\n");
        hasWeapon = 2;
    }
    else if (characterPath == 3)
    { // Scientist
        printf("You receive the Clockwork Lantern that can briefly freeze time, and an Etheric Grenade.\n");
        hasWeapon = 3;
    }

    printf("Additionally, you receive Tarek's Charm, which will boost your stamina in the coming trials.\n");
    hasTarekCharm = 1;

    printf("\nWant to proceed to Level 4? (y/n)\n");
    while (1)
    {
        choice = getPlayerChoice();

        if (choice == 'y')
        {
            gameProgress = 6;
            displayLevel4();
            return;
        }
        else if (choice == 'n')
        {
            printf("\nThank you for playing The Veil of Ash & Stars. Goodbye!\n");
            exit(0);
        }
        else
        {
            printf("Please enter y or n.\n");
        }
    }
}

void displayLevel4()
{
    currentLocation = 6;
    char choice;
    clear_screen();

    printf("\n\nLevel 4: The Right Hand Man's Ambush\n");
    printf("------------------------------------\n\n");
    printf("PLACE: A hidden fortress of obsidian spires, veiled in blood mist.\n");
    printf("The air vibrates with the cries of the corrupted.\n\n");
    printf("There you see villain's Right Hand, covered in blood.\n\n");
    printf("The Right Hand lieutenant: \"Ah...\nThe puppet arrives.\"\n\n");
    printf("You: \"Who are you? Where is the villain?\"\n\n");
    printf("The Right Hand lieutenant: \"Don't be restless puppet. Everything here is unknown to you.\"\n\n");

    printf("Choose your response:\n");
    printf("a. \"What is your name?\"\n");
    printf("b. \"I follow no one's tune.\"\n");
    printf("c. \"Where is your master?\"\n");
    printf("d. \"I will cut down anyone in my path.\"\n");

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case 'a':
            printf("\nYou ask: \"What is your name?\"\n\n");
            printf("The Right Hand lieutenant: \"I am Obaidul, your very own death. HAHAHAHAHA!\"\n\n");
            printf("You: \"You are so full of yourself.\"\n\n");
            break;

        case 'b':
            printf("\nYou declare: \"I follow no one's tune.\"\n\n");
            printf("Obaidul: \"BRAVATA! DELICIOSO! Tarek gave you those tools, but they were never yours. You wear your chains well, Seeker.\"\n\n");
            break;

        case 'c':
            printf("\nYou demand: \"Where is your master?\"\n\n");
            printf("Obaidul: \"BRAVATA! DELICIOSO! Tarek gave you those tools, but they were never yours. You wear your chains well, Seeker.\"\n\n");
            break;

        case 'd':
            printf("\nYou threaten: \"I will cut down anyone in my path.\"\n\n");
            printf("Obaidul doesn't reply to your threat.\n\n");
            break;

        default:
            printf("Please choose a valid option (a, b, c, or d).\n");
            continue;
        }
        break;
    }

    printf("(Combat begins. The mask breaks illusions. Victory is brutal.)\n\n");

    // Combat options based on character path
    printf("Choose your action:\n");
    if (hasWeapon == 1)
    { // Scholar with Tome
        printf("a. Use Tome - \"The words burn your tongue as you speak them\"\n");
    }
    else if (hasWeapon == 2)
    { // Knight with Sword
        printf("a. Use Sword - \"It feels heavier than usual... almost reluctant\"\n");
    }
    else if (hasWeapon == 3)
    { // Scientist with Lantern
        printf("a. Use Lantern - \"The gears stutter. Something's interfering\"\n");
    }
    printf("b. Dodge the attack\n");
    printf("c. Listen to Obaidul's words\n");

    while (1)
    {
        choice = getPlayerChoice();

        if (choice == 'a' || choice == 'b' || choice == 'c')
        {
            printf("\nYou engage in a fierce battle with Obaidul.\n\n");

            printf("Every round, Obaidul whispers: \"Why fight? He will just bring you back like the others.\"\n");
            printf("(Flash of memory - dozens of versions of you, all dead in this same room)\n\n");

            combat1(); // obaidul nigga

            printf("After a grueling battle, you finally defeat Obaidul.\n\n");

            printf("Obaidul (dying words): \"He watches from the other side. You trust him… but he is the gate and the lock.\"\n\n");
            printf("You have obtained Obaidul's Mask, which reveals illusions!\n");
            hasObaidulMask = 1;
            break;
        }
        else
        {
            printf("Please choose a valid option.\n");
        }
    }

    printf("You also find the key to the enemy sanctum.\n");
    printf("You feel stronger than before.\n\n");

    printf("\nWant to proceed to Level 5? (y/n)\n");
    while (1)
    {
        choice = getPlayerChoice();

        if (choice == 'y')
        {
            gameProgress = 7;
            displayLevel5();
            return;
        }
        else if (choice == 'n')
        {
            printf("\nThank you for playing The Veil of Ash & Stars. Goodbye!\n");
            printf("Exiting...\n");
            _sleep(1500);
            exit(0);
        }
        else
        {
            printf("Please enter y or n.\n");
        }
    }
}

void displayLevel5()
{
    currentLocation = 7;
    char choice;
    clear_screen();

    printf("\n\nLevel 5: The Veiled Deceiver");
    printf("\n-----------------------------\n\n");
    printf("PLACE: A storm of stars surrounds a throne of bones. The throne room is a mirror maze.\n");
    printf("Every reflection shows a different version of you - some victorious, most dead.\n");
    printf("Tarek stands beside the most hated villain in history, Sheikh Hasina,\n");
    printf("the vicious woman who killed people uncountably for her own joy, her hood falling to reveal burning eyes.\n\n");

    printf("SHEIKH HASINA: \"Ah! How many did I kill? Names? Faces? I don't waste memory on vermin.\n");
    printf("Once you're down, you're just another stain on the floor.\"\n\n");

    printf("You: \"What do you mean?\"\n\n");

    printf("(The villain sighs like a disappointed parent)\n\n");

    printf("TAREK: \"You did well, Seeker. Too well. You wore every chain I handed you.\n");
    printf("Every 'gift' was a mark of control. And now… strike, if you dare.\"\n\n");

    printf("Choose your response:\n");
    printf("a. \"You betrayed me!\"\n");
    printf("b. \"What are these paths?\"\n");
    printf("c. \"I'll end this—now!\"\n");

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case 'a':
            printf("\nYou shout: \"You betrayed me!\"\n\n");
            break;

        case 'b':
            printf("\nYou ask: \"What are these paths?\"\n\n");
            break;

        case 'c':
            printf("\nYou declare: \"I'll end this—now!\"\n\n");
            break;

        default:
            printf("Please choose a valid option (a, b, or c).\n");
            continue;
        }
        break;
    }

    printf("SHEIKH HASINA: \"You think this is a game you can win? I am not bound by your choices. Your mind is the battlefield.\"\n\n");

    printf("Tarek offers you three paths:\n");
    printf("a. Attack the villain directly\n");
    printf("b. Destroy the priest\n");
    printf("c. Flee\n");
    printf("d. Something is trying to tell you something. Want to hear it?\n");

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case 'a':
            printf("\nYou attack the villain directly.\n");
            printf("You lose. Her form was an illusion.\n\n");
            printf("GAME OVER\n");
            gameOver();
            return;

        case 'b':
            printf("\nYou attack the priest.\n");
            printf("The villain's power grows.\n\n");
            printf("GAME OVER\n");
            gameOver();
            return;

        case 'c':
            printf("\nYou attempt to flee.\n");
            printf("The veil collapses.\n\n");
            printf("GAME OVER\n");
            gameOver();
            return;

        case 'd':
            printf("\nYou pause and listen carefully...\n\n");
            printf("Unknown Voice (faint whisper): \"The mask… wear it. The compass… follow it. He's not in front of you.\"\n\n");

            if (hasObaidulMask && hasCelestialCompass)
            {
                printf("You put on Obaidul's Mask and see the Celestial Compass glowing.\n");
                printf("You turn around...\n\n");
                printf("The true vicious villain, SHEIKH HASINA rises behind you, breaking free from the veil.\n\n");
                printf("[Final Confrontation]: Fight Begins.\n\n");

                combat2(); // final fight here nigga
                printf("After an epic battle, you emerge victorious!\n\n");

                printf("SHEIKH HASINA [fading]: \"You… broke the cycle… Watcher…\"\n\n");
                printf("TAREK [fading]: \"Even in death… I still wait… in the next veil…\"\n\n");
                sleep_ms(3000); // Pause for 3 seconds

                gameProgress = 8;
                displayEpilogue();
                return;
            }
            else
            {
                printf("You feel you're missing something important...\n");
                printf("You need both Obaidul's Mask and the Celestial Compass to see the truth.\n\n");
                printf("GAME OVER\n");
                gameOver();
                return;
            }
            break;

        default:
            printf("Please choose a valid option (a, b, c, or d).\n");
            continue;
        }
    }
}

void displayEpilogue()
{
    currentLocation = 8;
    char choice;
    clear_screen();

    printf("\n\nEpilogue: The Veil's Judgement\n");
    printf("-----------------------------\n\n");
    printf("[Unknown Voice]: \"You chose wisely. The world is not saved, it is changed. Now, you must choose what it becomes.\"\n\n");

    printf("Make your final choice:\n");
    printf("a. Trust blindly\n");
    printf("b. Question but fail\n");
    printf("c. Uncover the truth\n");

    while (1)
    {
        choice = getPlayerChoice();

        switch (choice)
        {
        case 'a':
            printf("\nYou trusted blindly. The world burns.\n\n");
            printf("BAD ENDING: \"The Eternal Loop\"\n");
            printf("The priest's last trick is a lie to you. The veil falls away.\n");
            printf("You are reborn to continue the cycle endlessly after the world resets.\n\n");
            break;

        case 'b':
            printf("\nYou questioned but failed. The Veil flickers, hope remains.\n\n");
            printf("NEUTRAL ENDING\n");
            printf("The cycle continues, but a glimmer of hope remains for the future.\n\n");
            break;

        case 'c':
            printf("\nYou uncovered the truth and became a new Watcher.\n\n");
            printf("TRUE ENDING: \"The Crowned Seeker\"\n");
            printf("You uncover the truth, defeat the deceiver. The world is safe.\n");
            printf("You got the Watcher's crown and are assigned as the new guardian of the veil.\n\n");
            break;

        default:
            printf("Please choose a valid option (a, b, or c).\n");
            continue;
        }
        break;
    }

    printf("\nTHE END\n\n");
    printf("Thank you for playing The Veil of Ash & Stars!\n\n");
    printf("Would you like to play again? (y/n)\n");

    while (1)
    {
        choice = getPlayerChoice();

        if (choice == 'y')
        {
            // Reset game state
            gameProgress = 0;
            characterPath = 0;
            currentLocation = 0;
            hasWatchersKey = 0;
            hasSigilOfClarity = 0;
            hasObaidulMask = 0;
            hasCelestialCompass = 0;
            hasStrengthBonus = 0;
            hasPuzzleLens = 0;
            hasTarekCharm = 0;
            hasWeapon = 0;

            // Start a new game
            displayPrologue();
            return;
        }
        else if (choice == 'n')
        {
            printf("\nThank you for playing The Veil of Ash & Stars. Goodbye!\n");
            exit(0);
        }
        else
        {
            printf("Please enter y or n.\n");
        }
    }
}

int main()
{
    int choice;
    int authenticated = 0;

    while (!authenticated)
    {
        printf("==================================\n");
        printf("   THE VEIL OF ASH & STARS\n");
        printf("==================================\n\n");
        printf("1. Sign Up\n2. Login\nEnter your choice: "); // Login Choices
        scanf("%d", &choice);

        if (choice == 1)
        {
            registerUser(); // calling registerUser() function from login.h
        }
        else if (choice == 2)
        {
            int loginAttempts = 0;
            const int maxAttempts = 3;

            while (loginAttempts < maxAttempts)
            {
                if (loginUser())
                { // calling loginUser() function from login.h
                    printf("Login successful! Welcome back.\n");

                    authenticated = 1;
                    break;
                }
                else
                {
                    loginAttempts++;
                    printf("\nInvalid username or password. Try again.\n");

                    if (loginAttempts == maxAttempts)
                    {
                        printf("Maximum login attempts reached. Please try later.\n");
                    }
                }
            }
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }

    while (1)
    {
        char choice1;

        printf("==================================\n");
        printf("   THE VEIL OF ASH & STARS\n");
        printf("==================================\n\n");
        printf("1. New Game\n");
        printf("2. Load Game\n");
        printf("3. Quit\n");

        scanf(" %c", &choice1);
        while (getchar() != '\n')
            ; // clearing input buffer

        switch (choice1)
        {
        case '1':
            displayPrologue();
            break;
        case '2':
            loadGame();
            break;
        case '3':
            printf("\nThank you for playing The Veil of Ash & Stars. Goodbye!\n");
            return 0;
        case 'm':
            printf("You need to start or load a game first.\n");
            break;
        case 'h':
            displayMenu();
            break;
        case 'q':
            printf("\nThank you for playing The Veil of Ash & Stars. Goodbye!\n");
            return 0;
        default:
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            break;
        }
    }
}