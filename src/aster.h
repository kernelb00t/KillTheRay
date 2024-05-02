#ifndef H_ASTER
#define H_ASTER
#include "raylib.h"
#include "globals.h"

typedef struct Aster {
  Vector2 pos;
  Vector2 vel;
  float size;
} Aster;

bool aster_update(Aster* self);

typedef struct ListAster {
  Aster data[MAX_ASTEROID];
  int head;
  int top;
} ListAster;

void list_aster_append(ListAster* self, Aster element);
void list_aster_erase(ListAster* self, int index);
int list_aster_size(ListAster* self);
void list_aster_clear(ListAster* self);

extern ListAster asters;

void spawnAster(Vector2 pos, Vector2 vel, float size);

#endif /* H_ASTER */
