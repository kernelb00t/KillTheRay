#include "raylib.h" // Importation de RayLib, la librairie graphique de ce projet
#include "raymath.h"
// #include  // Liste en C++ (NON CAR WASM)

template <class T, int count> struct List {
  T arr[count];
  int head = 0;
  int top = 0;
  // Ca risque de faire des trucs chelou
  void push_back(T element) {
    if (top < count) {
      arr[top++] = element;
      head = top;
    } else {
      if (head >= count) {
        head = 0;
      }
      arr[head++] = element;
    }
  }
  void erase(int index) {
    for (int i = index; i < top - 1; i++) {
      arr[i] = arr[i + 1];
    }
    top--;
    if (head > index)
      head--;
  }
  T &operator[](int index) { return arr[index]; }
  int size() { return top; }
  void clear() {
    top = 0;
    head = 0;
  }
};

float XMAX = 1200.0f; // Initialisation des variables de la taille de l'écran
float YMAX = 700.0f;
const float SIZE = 1.f;           // Taille du joueur
const float ASTER_SPEED = 56.f;   // Vitesse d'un astéroide
const float BULLET_SPEED = 360.f; // Vitess d'un bullet
const float ANGLE_SPEED = 4.f;    // Vitesse angulaire du joueur
const float MAX_SPEED = 240.f;    // Vitesse maximum du joueur
const float COOLDOWN = 0.5f;      // Temps réel entre chaque Bullet tiré
Vector2 mpo;                      // Position de la souris
Vector2 mdt;                      // Vittesse de celle-ci
float dt;                         // Initialisation du delta time du jeu
int score = 0;                    // Initialisation du score

// Déclaration du système de scènes
enum Scene { GAME, GAMEOVER };
Scene current_scene;

Vector2 rand_side() {
  // Fonction qui génère un vecteur Position à partir d'un côté aléatoire.s
  switch (GetRandomValue(0, 3)) {
  case 0: // Coté haut
    return Vector2{(float)GetRandomValue(0, XMAX), 0};
    break;

  case 1: // Coté droit
    return Vector2{XMAX, (float)GetRandomValue(0, YMAX)};
    break;

  case 2: // Coté bas
    return Vector2{(float)GetRandomValue(0, XMAX), YMAX};
    break;

  case 3: // Coté gauche
    return Vector2{0, (float)GetRandomValue(0, YMAX)};
    break;
  }
  return Vector2{0, 0};
}

Vector2 randv2(float scale = 1.f) {
  return Vector2Scale(
      {(float)GetRandomValue(-1000, 1000), (float)GetRandomValue(-1000, 1000)},
      scale * 0.001f);
}

struct Bullet {
  Vector2 pos;
  Vector2 vel;

  bool update() {
    pos = Vector2Add(pos, Vector2Scale(vel, dt));
    DrawLineV(pos, Vector2Add(pos, Vector2Scale(vel, 0.05f)), WHITE);
    if (!CheckCollisionPointRec(pos, {0, 0, XMAX, YMAX})) {
      return true;
    }
    return false;
  }
};

List<Bullet, 500> bullets;

struct Aster {
  Vector2 pos;
  Vector2 vel;
  float size;

  bool update();
};

List<Aster, 500> asters;

bool Aster::update() {
    pos = Vector2Add(pos, Vector2Scale(vel, dt));
    DrawCircleV(pos, size * 10.f, WHITE);
    for (int i = 0; i < (int)bullets.size(); i++) {
      if (CheckCollisionPointCircle(bullets[i].pos, pos, size * 10.f)) {
        score += 1;
        bullets.erase(i);
        i--;
        return true;
      }
    }
    for (int i = 0; i < (int)asters.size(); i++) {
      if (CheckCollisionCircles(asters[i].pos, asters[i].size * 10.f, pos, size * 10.f)) {
        const Vector2 sum_vel = Vector2Add(asters[i].vel, vel);
        const float sum_len = Vector2Length(sum_vel);
        const float ratio = float(asters[i].size)/size;
        const Vector2 temp = asters[i].vel;
        asters[i].vel = Vector2Scale(Vector2Normalize(vel), sum_len * ratio);
        vel = Vector2Scale(Vector2Normalize(temp), sum_len * (1.f - ratio));
        const Vector2 middle = Vector2Scale(Vector2Add(asters[i].pos, pos), 0.5f);
        asters[i].pos = Vector2Add(
          middle,
          Vector2Scale(
            Vector2Normalize(Vector2Subtract(asters[i].pos, middle)),
            size*10.f
          )
        );
        pos = Vector2Add(
          middle,
          Vector2Scale(
            Vector2Normalize(Vector2Subtract(pos, middle)),
            size*10.f
          )
        );
      }
    }

    if (pos.x < 0.f)
      pos.x = XMAX;
    if (pos.x > XMAX)
      pos.x = 0.f;
    if (pos.y < 0.f)
      pos.y = YMAX;
    if (pos.y > YMAX)
      pos.y = 0.f;
    return false;
  }

void spawnBullet(Vector2 pos, Vector2 vel) {
  bullets.push_back(Bullet{pos, vel});
}

void spawnAster(Vector2 pos, Vector2 vel, float size) {
  asters.push_back(Aster{pos, vel, size});
}

struct Player {
  Vector2 pos;
  Vector2 vel = {0.f, 0.f};
  float angle = 0.f;
  float cooldown = 0.f;

  void update() {
    for (int i = 0; i < (int)asters.size(); i++) {
      if (CheckCollisionCircles(asters[i].pos, asters[i].size * 10.f, pos,
                                SIZE * 5.f)) {
        current_scene = GAMEOVER;
      }
    }
    Vector2 tri[3] = {{pos.x, pos.y - 10 * SIZE},
                      {pos.x - 5.f * SIZE, pos.y + 10.f * SIZE},
                      {pos.x + 5.f * SIZE, pos.y + 10.f * SIZE}};
    const float cos_theta = cosf(angle);
    const float sin_theta = sinf(angle);
    for (int i = 0; i < 3; i++) {
      tri[i] = {cos_theta * (tri[i].x - pos.x) -
                    sin_theta * (tri[i].y - pos.y) + pos.x,
                sin_theta * (tri[i].x - pos.x) +
                    cos_theta * (tri[i].y - pos.y) + pos.y};
    }
    DrawTriangle(tri[0], tri[1], tri[2], WHITE);
    int keydown = IsKeyDown(KEY_DOWN);
    vel = Vector2Add(
        vel, Vector2Scale({cosf(angle - PI / 2.f), sinf(angle - PI / 2.f)},
                          (IsKeyDown(KEY_UP)) * dt * 480.f));
    float length = Vector2Length(vel);
    if (length > MAX_SPEED) {
      vel = Vector2Scale(vel, MAX_SPEED / length);
    }
    if (keydown) {
      vel = Vector2Scale(vel, 0.001f / dt);
    }
    angle += (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) * dt * ANGLE_SPEED;
    pos = Vector2Add(pos, Vector2Scale(vel, dt));

    if (cooldown <= 0.f) {
      if (IsKeyDown(KEY_SPACE)) {
        spawnBullet(
            pos, Vector2Scale({cosf(angle - PI / 2.f), sinf(angle - PI / 2.f)},
                              BULLET_SPEED));
        cooldown = COOLDOWN;
      }
    } else {
      cooldown -= dt;
    }

    if (pos.x < 0.f)
      pos.x = XMAX;
    if (pos.x > XMAX)
      pos.x = 0.f;
    if (pos.y < 0.f)
      pos.y = YMAX;
    if (pos.y > YMAX)
      pos.y = 0.f;
  }
} player;

void reset() {
  player.vel = {0.f, 0.f};
  player.pos = {XMAX / 2.f, YMAX / 2.f};
  score = 0;
  bullets.clear();
  asters.clear();
  for (int i = 0; i < 4; i++) {
    spawnAster(rand_side(), randv2(ASTER_SPEED), GetRandomValue(2, 5));
  }
}

int main() {
  SetTargetFPS(60);
  // SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(XMAX, YMAX, "Name of the window here.");
  XMAX = GetScreenWidth();
  YMAX = GetScreenHeight();
  // No need to alloc mem coz wasm doesn't have heap
  // bullets.reserve(120);
  // asters.reserve(120);
  reset();

  const float TIMING = 5.f;
  float timing = TIMING;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (current_scene) {
    case GAME: {

      if (timing <= 0.f) {
        spawnAster(rand_side(), randv2(ASTER_SPEED), GetRandomValue(2, 5));
        timing = TIMING;
      } else {
        timing -= dt;
      }

      player.update();
      for (int i = 0; i < (int)bullets.size(); i++) {
        if (bullets[i].update()) {
          bullets.erase(i);
          i--;
        }
      }
      for (int i = 0; i < (int)asters.size(); i++) {
        if (asters[i].update()) {
          if (asters[i].size > 1.f) {
            spawnAster(asters[i].pos, randv2(10.f), asters[i].size / 2.f);
            spawnAster(asters[i].pos, randv2(10.f), asters[i].size / 2.f);
          }
          asters.erase(i);
          i--;
        }
      }
      DrawText(TextFormat("Score: %i", score), 0, 30, 30, WHITE);
      DrawText(TextFormat("Speed: %.2f", Vector2Length(player.vel)), 0, 60, 30,
               WHITE);

      break;
    }
    case GAMEOVER: {
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
      break;
    }
    }
    DrawFPS(0, 0);
    EndDrawing();

    mpo = GetMousePosition();
    mdt = GetMouseDelta();
    dt = GetFrameTime();
  }

  CloseWindow();
}
