#ifndef H_SCENES
#define H_SCENES

typedef enum Scene { GAME, GAMEOVER, SCENE_COUNT } Scene;
extern Scene current_scene;
extern void update_current_scene();
extern void reset();

#endif /* H_SCENES */
