#pragma once

#include <gui/scene_manager.h>


#define ADD_SCENE(prefix, name, id) DuckyGenScene##id,
typedef enum {
#include "ducky_gen_scene_config.h"
  DuckyGenSceneNum,
} DuckyGenScene;
#undef ADD_SCENE

extern const SceneManagerHandlers ducky_gen_scene_handlers;


#define ADD_SCENE(prefix, name, id)                                            \
  void prefix##_scene_##name##_on_enter(void *context);                        \
  bool prefix##_scene_##name##_on_event(void *context,                         \
                                        SceneManagerEvent event);              \
  void prefix##_scene_##name##_on_exit(void *context);
#include "ducky_gen_scene_config.h"
#undef ADD_SCENE
