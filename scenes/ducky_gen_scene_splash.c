#include "../ducky_gen_app_i.h"

#define SPLASH_TIMEOUT_TICKS                                                   \
  20

void ducky_gen_scene_splash_on_enter(void *context) {
  DuckyGenApp *app = context;


  view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewSplash);
}

bool ducky_gen_scene_splash_on_event(void *context, SceneManagerEvent event) {
  DuckyGenApp *app = context;

  if (event.type == SceneManagerEventTypeTick) {

    splash_view_timer_tick(app->splash_view);






    uint32_t state =
        scene_manager_get_scene_state(app->scene_manager, DuckyGenSceneSplash);
    state++;
    scene_manager_set_scene_state(app->scene_manager, DuckyGenSceneSplash,
                                  state);

    if (state >= SPLASH_TIMEOUT_TICKS) {
      scene_manager_next_scene(app->scene_manager, DuckyGenSceneMainMenu);
      return true;
    }
  }
  return false;
}

void ducky_gen_scene_splash_on_exit(void *context) {
  DuckyGenApp *app = context;

  scene_manager_set_scene_state(app->scene_manager, DuckyGenSceneSplash, 0);
}
