#include "aster.h"
#include "globals.h"
#include "bullet.h"
#include "raymath.h"

ListAster asters;

bool aster_update(Aster* self) {
  self->pos = Vector2Add(self->pos, Vector2Scale(self->vel, dt * (1.f + 1.f/self->size)));
  DrawCircleV(self->pos, self->size * 10.f, WHITE);
  for (int i = 0; i < list_bullet_size(&bullets); i++) {
    if (CheckCollisionPointCircle(bullets.data[i].pos, self->pos, self->size * 10.f)) {
      score += 1;
      list_bullet_erase(&bullets, i);
      i--;
      return true;
    }
  }
  // NOTE - This is disabled because it doesn't work for now
  // for (int i = 0; i < (int)asters.size(); i++) {
  //   if ((asters.arr + i) == this)
  //     continue;
  //   if (CheckCollisionCircles(asters[i].pos, asters[i].size * 10.f, self->pos, size * 10.f)) {
  //     const Vector2 sum_vel = Vector2Add(asters[i].vel, vel);
  //     const float sum_len = Vector2Length(sum_vel);
  //     const float ratio = float(asters[i].size)/size;
  //     const Vector2 temp = asters[i].vel;
  //     asters[i].vel = Vector2Scale(Vector2Normalize(vel), sum_len * ratio);
  //     vel = Vector2Scale(Vector2Normalize(temp), sum_len * (1.f - ratio));
  //     const Vector2 middle = Vector2Scale(Vector2Add(asters[i].pos, self->pos), 0.5f);
  //     asters[i].pos = Vector2Add(
  //       middle,
  //       Vector2Scale(
  //         Vector2Normalize(Vector2Subtract(asters[i].pos, middle)),
  //         asters[i].size*10.f
  //       )
  //     );
  //     pos = Vector2Add(
  //       middle,
  //       Vector2Scale(
  //         Vector2Normalize(Vector2Subtract(pos, middle)),
  //         size*10.f
  //       )
  //     );
  //   }
  // }

  if (self->pos.x < 0.f)
    self->pos.x = XMAX;
  if (self->pos.x > XMAX)
    self->pos.x = 0.f;
  if (self->pos.y < 0.f)
    self->pos.y = YMAX;
  if (self->pos.y > YMAX)
    self->pos.y = 0.f;
  return false;
}

void list_aster_append(ListAster* self, Aster element) {
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

void list_aster_erase(ListAster* self, int index) {
  for (int i = index; i < self->top - 1; i++) {
    self->data[i] = self->data[i + 1];
  }
  self->top--;
  if (self->head > index)
    self->head--;
}

int list_aster_size(ListAster* self) {
  return self->top;
}

void list_aster_clear(ListAster* self) {
  self->top = 0;
  self->head = 0;
}

void spawnAster(Vector2 pos, Vector2 vel, float size) {
  list_aster_append(&asters, (Aster){pos, vel, size});
}
