#ifndef H_SCENES
#define H_SCENES

typedef enum Scene { GAME, GAMEOVER, SCENE_COUNT } Scene;
extern Scene current_scene;
void update_current_scene();
void reset();

#endif /* H_SCENES */
