#include "bullet.h"
#include "globals.h"
#include "raymath.h"
#include "raylib.h"

ListBullet bullets;

bool bullet_update(Bullet* self) {
    self->pos = Vector2Add(self->pos, Vector2Scale(self->vel, dt));
    DrawLineV(
        self->pos,
        Vector2Add(self->pos, Vector2Scale(self->vel, 0.05f)),
        WHITE
    );
    if (!CheckCollisionPointRec(self->pos, (Rectangle){0, 0, XMAX, YMAX})) {
        return true;
    }
    return false;
}

void list_bullet_append(ListBullet* self, Bullet element) {
    if (self->top < MAX_BULLET) {
        self->data[self->top++] = element;
        self->head = self->top;
    } else {
        if (self->head >= MAX_BULLET) {
            self->head = 0;
        }
        self->data[self->head++] = element;
    }
}

void list_bullet_erase(ListBullet* self, int index) {
  for (int i = index; i < self->top - 1; i++) {
    self->data[i] = self->data[i + 1];
  }
  self->top--;
  if (self->head > index)
    self->head--;
}

int list_bullet_size(ListBullet* self) {
  return self->top;
}

void list_bullet_clear(ListBullet* self) {
  self->top = 0;
  self->head = 0;
}

void spawnBullet(Vector2 pos, Vector2 vel) {
  list_bullet_append(&bullets, (Bullet){pos, vel});
}
