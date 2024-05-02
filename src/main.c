#include "raylib.h"
#include "raymath.h"
#include "bullet.h"
#include "aster.h"
#include "scenes.h"

int main() {
  SetTargetFPS(60);
  // SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(XMAX, YMAX, "KillTheRay");
  XMAX = GetScreenWidth();
  YMAX = GetScreenHeight();
  reset();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    update_current_scene();

    DrawFPS(0, 0);
    EndDrawing();

    mpo = GetMousePosition();
    mdt = GetMouseDelta();
    dt = GetFrameTime();
  }

  CloseWindow();
}
