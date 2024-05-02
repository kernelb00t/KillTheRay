#include "scenes.h"
#include "player.h"
#include "globals.h"
#include "bullet.h"
#include "aster.h"
#include "utils.h"
#include "raymath.h"

typedef void(*scene_func_t)();

void reset() {
  player.vel = (Vector2){0.f, 0.f};
  player.pos = (Vector2){XMAX / 2.f, YMAX / 2.f};
  score = 0;
  list_bullet_clear(&bullets);
  list_aster_clear(&asters);
  for (int i = 0; i < 4; i++) {
    spawnAster(rand_side(), randv2(ASTER_SPEED), GetRandomValue(2, 5));
  }
}

const float TIMING = 5.f;
float timing = TIMING;

void scene_game() {
	if (timing <= 0.f) {
    spawnAster(rand_side(), randv2(ASTER_SPEED), GetRandomValue(2, 5));
    timing = TIMING;
  } else {
    timing -= dt;
  }

  player_update(&player);
  for (int i = 0; i < list_bullet_size(&bullets); i++) {
    if (bullet_update(&bullets.data[i])) {
      list_bullet_erase(&bullets, i);
      i--;
    }
  }
  for (int i = 0; i < list_aster_size(&asters); i++) {
    if (aster_update(&asters.data[i])) {
      if (asters.data[i].size > 1.f) {
        spawnAster(asters.data[i].pos, randv2(10.f), asters.data[i].size / 2.f);
        spawnAster(asters.data[i].pos, randv2(10.f), asters.data[i].size / 2.f);
      }
      list_aster_erase(&asters, i);
      i--;
    }
  }
  DrawText(TextFormat("Score: %i", score), 0, 30, 30, WHITE);
  DrawText(TextFormat("Speed: %.2f", Vector2Length(player.vel)), 0, 60, 30,
            WHITE);
}

void scene_gameover() {
  const char *fmt1 = TextFormat("Score: %i", score);
  const char *fmt2 = "YOU SUCK AT ASTEROID";
  float w1 = MeasureText(fmt1, 40);
  float w2 = MeasureText(fmt2, 70);
  DrawText(fmt1, XMAX / 2 - w1 / 2, 30, 40, WHITE);
  DrawText(fmt2, XMAX / 2 - w2 / 2, 120, 70, WHITE);
  if (IsKeyPressed(KEY_SPACE)) {
    reset();
    current_scene = GAME;
  }
}

scene_func_t scenes[SCENE_COUNT] = {
	scene_game,
	scene_gameover
};

Scene current_scene;
void update_current_scene() {
	scenes[current_scene]();
}
