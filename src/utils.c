#include "utils.h"
#include "raylib.h"
#include "raymath.h"
#include "globals.h"

Vector2 rand_side() {
  // Fonction qui génère un vecteur Position à partir d'un côté aléatoire.s
  switch (GetRandomValue(0, 3)) {
  case 0: // Coté haut
    return (Vector2){(float)GetRandomValue(0, XMAX), 0};
    break;

  case 1: // Coté droit
    return (Vector2){XMAX, (float)GetRandomValue(0, YMAX)};
    break;

  case 2: // Coté bas
    return (Vector2){(float)GetRandomValue(0, XMAX), YMAX};
    break;

  case 3: // Coté gauche
    return (Vector2){0, (float)GetRandomValue(0, YMAX)};
    break;
  }
  return (Vector2){0, 0};
}

Vector2 randv2(float scale) {
  return Vector2Scale(
      (Vector2){(float)GetRandomValue(-1000, 1000), (float)GetRandomValue(-1000, 1000)},
      scale * 0.001f);
}

