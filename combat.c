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
    {"Obsidian Crush", 20, 1, 0, 1, 0, 2.0}, // Stuns the player and deals double damage if not evaded
    {"Dark Reprisal", 15, 0, 0, 0, 3, 1.0},  // Deals damage and applies burn for 3 turns
    {"Thronebreaker", 0, 3, 0, 0, 0, 1.0},   // Reduces player HP to 20% and applies broken will for 3 turns
    {"Echoing Rend", 10, 0, 0, 0, 0, 1.0},   // Deals damage and applies echo wound effect
};

void resolve_enemy_move(Entity *enemy, Entity *player, int move)
{
    EnemyMove selected_move = enemy_moves[move - 1]; // Move index is 1-based
    printf("%s uses %s!\n", enemy->name, selected_move.name);

    if (player->is_evading)
    {
        printf("%s evaded the attack!\n", player->name);
        return;
    }

    // Calculate total damage with multiplier
    int total_damage = (int)(selected_move.base_damage * selected_move.damage_multiplier);

    // Apply damage
    if (total_damage > 0)
    {
        player->hp -= total_damage;
        printf("%s takes %d damage!\n", player->name, total_damage);
    }

    // Apply status effects
    if (selected_move.stun_duration > 0)
    {
        player->stunned = selected_move.stun_duration;
        printf("%s is stunned for %d turns!\n", player->name, selected_move.stun_duration);
    }

    if (selected_move.poison_duration > 0)
    {
        player->poisoned_turns = selected_move.poison_duration;
        printf("%s is poisoned for %d turns!\n", player->name, selected_move.poison_duration);
    }

    if (selected_move.heal_block_duration > 0)
    {
        player->broken_will_turns = selected_move.heal_block_duration;
        printf("%s's healing will be blocked for %d turns!\n", player->name, selected_move.heal_block_duration);
    }

    if (selected_move.burn_duration > 0)
    {
        player->black_burn_turns = selected_move.burn_duration;
        printf("%s is burned for %d turns!\n", player->name, selected_move.burn_duration);
    }
}

void apply_status_effects(Entity *e)
{
    if (e->poisoned_turns > 0)
    {
        printf("%s suffers poison damage!\n", e->name);
        e->hp -= 5;
        e->poisoned_turns--;
    }
    if (e->empowered_turns > 0)
    {
        e->empowered_turns--;
    }
    if (e->stunned > 0)
    {
        e->stunned--;
    }
    if (e->skip_dodge > 0)
    {
        e->skip_dodge--;
    }
    if (e->echo_wound > 0)
    {
        e->echo_wound--;
    }
}

void resolve_player_action(Entity *player, Entity *enemy, int main_action, int attack_choice)
{
    if (player->stunned > 0)
    {
        printf("%s is stunned and loses this turn!\n", player->name);
        return;
    }

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
            printf("%s performs Crescent Cleave!\n", player->name);
            break;
        case 2:
            base_damage = 25;
            enemy->stunned = 1;
            printf("%s performs Overhead Crush! %s is stunned!\n", player->name, enemy->name);
            break;
        case 3:
            base_damage = 10;
            enemy->poisoned_turns = 3;
            printf("%s casts Rot Bloom! %s is poisoned!\n", player->name, enemy->name);
            break;
        case 4:
            base_damage = (enemy->hp < enemy->max_hp / 2) ? 30 : 15;
            printf("%s fires Eclipse Bolt!\n", player->name);
            break;
        default:
            printf("Invalid attack choice.\n");
            return;
        }

        if (enemy->is_evading)
        {
            printf("%s evaded the attack and took no damage!\n", enemy->name);
        }
        else
        {
            if (player->empowered_turns > 0)
                base_damage += 5;
            if (enemy->echo_wound > 0)
                base_damage = (int)(base_damage * 1.5);
            if (main_action == 2)
                base_damage /= 2;
            enemy->hp -= base_damage;
            printf("%s takes %d damage!\n", enemy->name, base_damage);
        }
        break;
    }
    case 3:
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

void resolve_enemy_action(Entity *enemy, Entity *player, int move)
{
    if (enemy->stunned > 0)
    {
        printf("%s is stunned and skips this turn!\n", enemy->name);
        return;
    }

    // Call the resolve_enemy_move function to handle the move
    resolve_enemy_move(enemy, player, move);
}

void battle_loop(Entity *player, Entity *enemy)
{
    int try_again = 1;

    while (try_again)
    {
        // Reset the player and enemy HP before starting a new battle
        player->hp = player->max_hp;
        enemy->hp = enemy->max_hp;

        while (player->hp > 0 && enemy->hp > 0)
        {
            apply_status_effects(player);
            apply_status_effects(enemy);

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
                printf("1) Crescent Cleave\n");
                printf("2) Overhead Crush\n");
                printf("3) Rot Bloom\n");
                printf("4) Eclipse Bolt\n> ");
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
            resolve_enemy_action(enemy, player, enemy_choice);
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
            sleep_ms(5000); // Wait for 5 seconds before asking to try again
        }
    }
}

void combat()
{
    Entity player = {"Hero", 100, 100, 20, 5, 0, 0, 3};
    Entity enemy = {"Grimm", 120, 120, 18, 4, 0, 0, 0};
    battle_loop(&player, &enemy);
}
