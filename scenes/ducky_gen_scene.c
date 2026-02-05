#include "ducky_gen_scene.h"


#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const ducky_gen_scene_on_enter_handlers[])(void *) = {
#include "ducky_gen_scene_config.h"
};
#undef ADD_SCENE


#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const ducky_gen_scene_on_event_handlers[])(void *, SceneManagerEvent) = {
#include "ducky_gen_scene_config.h"
};
#undef ADD_SCENE


#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const ducky_gen_scene_on_exit_handlers[])(void *) = {
#include "ducky_gen_scene_config.h"
};
#undef ADD_SCENE

const SceneManagerHandlers ducky_gen_scene_handlers = {
    .on_enter_handlers = ducky_gen_scene_on_enter_handlers,
    .on_event_handlers = ducky_gen_scene_on_event_handlers,
    .on_exit_handlers = ducky_gen_scene_on_exit_handlers,
    .scene_num = DuckyGenSceneNum,
};
