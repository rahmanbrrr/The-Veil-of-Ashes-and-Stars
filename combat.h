#ifndef COMBAT_H
#define COMBAT_H

// Minimal Entity struct — no status effects or buffs
typedef struct {
    const char *name;
    int hp;
    int max_hp;
    int attack;
    int defense;
    int is_defending;
    int is_evading;
    int inventory;
} Entity;

// Minimal EnemyMove struct — damage only
typedef struct {
    char name[50];       // Name of the move
    int base_damage;     // Base damage
} EnemyMove;

// Function declarations
int combat1();
int combat2();
void draw_health_bar(const char *name, int hp, int max_hp);
void handle_turn(Entity *player, Entity *enemy, int player_choice, int enemy_choice);
int battle_loop(Entity *player, Entity *enemy);
void resolve_player_action(Entity *player, Entity *enemy, int main_action, int attack_choice);
int max_val(int a, int b);
void sleep_ms(int milliseconds);

#endif
