#ifndef H_GLOBALS
#define H_GLOBALS
#include "raylib.h"

// Some configurations
#define SIZE 1.f           // Player's size
#define ASTER_SPEED 56.f   // Speed of an asteroid
#define BULLET_SPEED 360.f // Speed of a bullet
#define ANGLE_SPEED 4.f    // Player's angular speed
#define MAX_SPEED 240.f    // Maximum speed of the player
#define COOLDOWN 0.5f      // Real time cooldown between each bullet shots
#define MAX_BULLET 500
#define MAX_ASTEROID 500

extern float dt;
extern float XMAX; // Window's size
extern float YMAX;
extern Vector2 mpo; // Mouse position // REVIEW: remove? it's not used anywhere
extern Vector2 mdt; // Speed of the mouse
extern float dt; // Delta-time
extern int score; // The main indicator if you're good at this game or if you suck.

#endif /* H_GLOBALS */
