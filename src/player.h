#ifndef H_PLAYER
#define H_PLAYER
#include "raylib.h"

typedef struct Player {
  Vector2 pos;
  Vector2 vel;
  float angle;
  float cooldown;
} Player;

extern Player player;

void player_update(Player* self);

#endif /* H_PLAYER */
