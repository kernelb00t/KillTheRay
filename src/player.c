#include "player.h"
#include "aster.h"
#include "bullet.h"
#include "scenes.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>

inline void player_update(Player* self) {
  for (int i = 0; i < list_aster_size(&asters); i++) {
    if (CheckCollisionCircles(asters.data[i].pos, asters.data[i].size * 10.f, self->pos,
                              SIZE * 5.f)) {
      current_scene = GAMEOVER;
    }
  }
  Vector2 tri[3] = {{self->pos.x, self->pos.y - 10 * SIZE},
                    {self->pos.x - 5.f * SIZE, self->pos.y + 10.f * SIZE},
                    {self->pos.x + 5.f * SIZE, self->pos.y + 10.f * SIZE}};
  const float cos_theta = cosf(self->angle);
  const float sin_theta = sinf(self->angle);
  for (int i = 0; i < 3; i++) {
    tri[i] = (Vector2){cos_theta * (tri[i].x - self->pos.x) -
                  sin_theta * (tri[i].y - self->pos.y) + self->pos.x,
              sin_theta * (tri[i].x - self->pos.x) +
                  cos_theta * (tri[i].y - self->pos.y) + self->pos.y};
  }
  DrawTriangle(tri[0], tri[1], tri[2], WHITE);
  int keydown = IsKeyDown(KEY_DOWN);
  self->vel = Vector2Add(
      self->vel, Vector2Scale((Vector2){cosf(self->angle - PI / 2.f), sinf(self->angle - PI / 2.f)},
                        (IsKeyDown(KEY_UP)) * dt * 480.f));
  float length = Vector2Length(self->vel);
  if (length > MAX_SPEED) {
    self->vel = Vector2Scale(self->vel, MAX_SPEED / length);
  }
  if (keydown) {
    self->vel = Vector2Scale(self->vel, 0.001f / dt);
  }
  self->angle += (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) * dt * ANGLE_SPEED;
  self->pos = Vector2Add(self->pos, Vector2Scale(self->vel, dt));

  if (self->cooldown <= 0.f) {
    if (IsKeyDown(KEY_SPACE)) {
      spawnBullet(
          self->pos, Vector2Scale((Vector2){cosf(self->angle - PI / 2.f), sinf(self->angle - PI / 2.f)},
                            BULLET_SPEED));
      self->cooldown = COOLDOWN;
    }
  } else {
    self->cooldown -= dt;
  }

  if (self->pos.x < 0.f)
    self->pos.x = XMAX;
  if (self->pos.x > XMAX)
    self->pos.x = 0.f;
  if (self->pos.y < 0.f)
    self->pos.y = YMAX;
  if (self->pos.y > YMAX)
    self->pos.y = 0.f;
}

Player player;
