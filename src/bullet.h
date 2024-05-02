#ifndef H_BULLET
#define H_BULLET
#include "raylib.h"
#include "globals.h"

typedef struct Bullet {
  Vector2 pos;
  Vector2 vel;
} Bullet;

bool bullet_update(Bullet* self);

typedef struct ListBullet {
  Bullet data[MAX_BULLET];
  int head;
  int top;
} ListBullet;

void list_bullet_append(ListBullet* self, Bullet element);
void list_bullet_erase(ListBullet* self, int index);
int list_bullet_size(ListBullet* self);
void list_bullet_clear(ListBullet* self);

extern ListBullet bullets;

void spawnBullet(Vector2 pos, Vector2 vel);

#endif /* H_BULLET */
