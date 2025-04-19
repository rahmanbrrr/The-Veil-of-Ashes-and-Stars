#include "combat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combat.h"
#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif
#include "clear.h"

int max_val(int a, int b)
{
    return (a > b) ? a : b;
}

// Example moveset for the enemy
EnemyMove enemy_moves[] = {
    {"\"she has made us fly\" attack", 20},
    {"\"Tumi ki kono dino bhalo hobena?\" attack", 15},
    {"\"Sheikh Hasina palay nah\" attack", 25},
    {"\"Bongoboltu more nai\" attack", 10},
};

void resolve_enemy_move(Entity *enemy, Entity *player, int move)
{
    EnemyMove selected_move = enemy_moves[move - 1]; // Move index is 1-based
    printf("%s uses %s!\n", enemy->name, selected_move.name);

    // Check if the player is evading, if so, skip the damage
    if (player->is_evading)
    {
        printf("%s evaded the attack!\n", player->name);
        return; // No damage if player evades
    }

    int total_damage = selected_move.base_damage;
    player->hp -= total_damage;
    printf("%s takes %d damage!\n", player->name, total_damage);
}

void resolve_player_action(Entity *player, Entity *enemy, int main_action, int attack_choice)
{
    switch (main_action)
    {
    case 1:
    case 2:
    {
        int base_damage = 0;
        if (main_action == 2)
            player->is_defending = 1;

        switch (attack_choice)
        {
        case 1:
            base_damage = 15;
            printf("%s performs \"Yunus Shorkar chai\" attack!", player->name);
            break;
        case 2:
            base_damage = 25;
            printf("%s performs \"Gopalganj ar thakbena\" attack\n", player->name);
            break;
        case 3:
            base_damage = 10;
            printf("%s casts \"areh, areh, AREH SHALAR***** MUNAJAAT DHOR\" attack\n", player->name);
            break;
        case 4:
            base_damage = (enemy->hp < enemy->max_hp / 2) ? 30 : 15;
            printf("%s fires \"Digital Bangladesh\" attack!\n", player->name);
            break;
        default:
            printf("Invalid attack choice.\n");
            return;
        }

        // If enemy is evading, no damage is done
        if (enemy->is_evading)
        {
            printf("%s evaded the attack and took no damage!\n", enemy->name);
            return;
        }

        // Apply damage if enemy is not evading
        enemy->hp -= base_damage;
        printf("%s takes %d damage!\n", enemy->name, base_damage);
        break;
    }
    case 3:
        // If the player chooses to evade, set the evade flag
        if (!player->is_evading) // Only show this if the player hasn't already evaded
        {
            printf("%s prepares to evade.\n", player->name);
        }
        player->is_evading = 1;
        break;

    case 4:
        if (player->inventory > 0)
        {
            int heal = 20;
            player->hp += heal;
            if (player->hp > player->max_hp)
                player->hp = player->max_hp;
            player->inventory--;
            printf("%s uses a potion and heals for %d HP.\n", player->name, heal);
        }
        else
        {
            printf("No potions left!\n");
        }
        break;
    default:
        printf("Invalid action.\n");
    }
}

int battle_loop(Entity *player, Entity *enemy)
{
    int try_again = 1;

    while (try_again)
    {
        // Reset the player and enemy HP before starting a new battle
        player->hp = player->max_hp;
        enemy->hp = enemy->max_hp;

        while (player->hp > 0 && enemy->hp > 0)
        {
            if (player->hp <= 0 || enemy->hp <= 0)
                break;

            printf("\n-- Combat Status --\n");
            printf("%s: %d/%d HP\tPotions: %d\n", player->name, player->hp, player->max_hp, player->inventory);
            printf("%s: %d/%d HP\n", enemy->name, enemy->hp, enemy->max_hp);

            int main_action = 0, attack_choice = 0;
            printf("\nChoose your action:\n");
            printf("1) Attack\n");
            printf("2) Defend & Attack\n");
            printf("3) Evade\n");
            printf("4) Heal\n> ");
            scanf("%d", &main_action);

            if (main_action == 1 || main_action == 2)
            {
                printf("\nChoose your attack:\n");
                printf("1) \"Yunus Shorkar chai\" attack\n");
                printf("2) \"Gopalganj ar thakbena\" attack \n");
                printf("3) \"areh, areh, AREH SHALAR***** MUNAJAAT DHOR\" attack\n");
                printf("4) \"Digital Bangladesh\" attack ");
                scanf("%d", &attack_choice);
            }

            int enemy_choice = rand() % 4 + 1;

            // Pre-process player's evade/defend intent
            if (main_action == 2)
                player->is_defending = 1;
            if (main_action == 3)
                player->is_evading = 1;

            // Pre-process enemy's evade/defend intent
            if (enemy_choice == 2)
                enemy->is_defending = 1;
            if (enemy_choice == 3)
                enemy->is_evading = 1;

            clear_screen();
            if (enemy->hp <= 0)
                break;

            // Resolve the enemy's action first
            // **Prevent enemy from attacking if evading**
            if (!enemy->is_evading)
            {
                resolve_enemy_move(enemy, player, enemy_choice);
            }
            else
            {
                printf("%s evades the attack!\n", enemy->name);
            }

            // Resolve the player's action second
            resolve_player_action(player, enemy, main_action, attack_choice);

            // Reset defenses and evasion after both turns
            player->is_defending = 0;
            player->is_evading = 0;
            enemy->is_defending = 0;
            enemy->is_evading = 0;
        }

        printf("\n-- Battle Ended --\n");
        if (player->hp <= 0 && enemy->hp <= 0)
        {
            printf("Both combatants have fallen...\n");
            sleep_ms(5000); // Wait for 5 seconds before asking to try again
        }
        else if (player->hp <= 0)
        {
            printf("You have been defeated.\n");

            char retry_input;
            printf("Try again (y/n)?\n> ");
            scanf(" %c", &retry_input); // Note the space before %c to capture any leftover newline

            if (retry_input == 'n' || retry_input == 'N')
            {
                try_again = 0; // Exit the loop and end the battle
            }
            // If 'y' or 'Y', the battle will restart automatically since try_again stays 1
            sleep_ms(5000); // Wait for 5 seconds before restarting the battle
        }
        else
        {
            printf("You have defeated %s!\n", enemy->name);
            return 0;
            sleep_ms(5000); // Wait for 5 seconds before asking to try again
        }
    }
}

int combat1()
{
    Entity player = {"Hero", 100, 100, 20, 5, 0, 0, 3};
    Entity enemy = {"Obaidul", 120, 120, 18, 4, 0, 0, 0};
    return battle_loop(&player, &enemy);
}

int combat2()
{
    Entity player = {"Hero", 100, 100, 20, 5, 0, 0, 3};
    Entity enemy = {"Hasina", 120, 120, 18, 4, 0, 0, 0};
    return battle_loop(&player, &enemy);
}
