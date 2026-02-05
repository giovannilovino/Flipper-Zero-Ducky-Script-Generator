#include "../ducky_gen_app_i.h"


enum {
  ModIndexCtrl,
  ModIndexAlt,
  ModIndexShift,
  ModIndexGui,
  ModIndexDelete,
  ModIndexNext,
};

static void builder_update_submenu(DuckyGenApp *app);

void ducky_gen_scene_modifier_builder_submenu_callback(void *context,
                                                       uint32_t index) {
  DuckyGenApp *app = context;

  if (index == ModIndexNext) {

    scene_manager_next_scene(
        app->scene_manager,
        DuckyGenSceneModifierKeySelect);
    return;
  }







  char *target = NULL;
  if (index == ModIndexCtrl)
    target = "CTRL";
  else if (index == ModIndexAlt)
    target = "ALT";
  else if (index == ModIndexShift)
    target = "SHIFT";
  else if (index == ModIndexGui)
    target = "GUI";
  else if (index == ModIndexDelete)
    target = "DELETE";

  if (target) {
    if (strstr(app->modifier_str, target)) {








      uint32_t mask = scene_manager_get_scene_state(
          app->scene_manager, DuckyGenSceneModifierBuilder);
      mask ^= (1 << index);
      scene_manager_set_scene_state(app->scene_manager,
                                    DuckyGenSceneModifierBuilder, mask);
      builder_update_submenu(app);
    } else {
      uint32_t mask = scene_manager_get_scene_state(
          app->scene_manager, DuckyGenSceneModifierBuilder);
      mask |= (1 << index);
      scene_manager_set_scene_state(app->scene_manager,
                                    DuckyGenSceneModifierBuilder, mask);
      builder_update_submenu(app);
    }
  }
}

static void builder_update_submenu(DuckyGenApp *app) {
  submenu_reset(app->submenu);

  uint32_t mask = scene_manager_get_scene_state(app->scene_manager,
                                                DuckyGenSceneModifierBuilder);


  app->modifier_str[0] = '\0';
  if (mask & (1 << ModIndexCtrl))
    strlcat(app->modifier_str, "CTRL ", sizeof(app->modifier_str));
  if (mask & (1 << ModIndexAlt))
    strlcat(app->modifier_str, "ALT ", sizeof(app->modifier_str));
  if (mask & (1 << ModIndexShift))
    strlcat(app->modifier_str, "SHIFT ", sizeof(app->modifier_str));
  if (mask & (1 << ModIndexGui))
    strlcat(app->modifier_str, "GUI ", sizeof(app->modifier_str));
  if (mask & (1 << ModIndexDelete))
    strlcat(app->modifier_str, "DELETE ", sizeof(app->modifier_str));


  char header[128];
  if (strlen(app->modifier_str) > 0) {
    snprintf(header, sizeof(header), "Mods: %s", app->modifier_str);
  } else {
    snprintf(header, sizeof(header), "Select Modifiers");
  }
  submenu_set_header(app->submenu, header);


  submenu_add_item(
      app->submenu, (mask & (1 << ModIndexCtrl)) ? "[X] CTRL" : "[ ] CTRL",
      ModIndexCtrl, ducky_gen_scene_modifier_builder_submenu_callback, app);
  submenu_add_item(
      app->submenu, (mask & (1 << ModIndexAlt)) ? "[X] ALT" : "[ ] ALT",
      ModIndexAlt, ducky_gen_scene_modifier_builder_submenu_callback, app);
  submenu_add_item(
      app->submenu, (mask & (1 << ModIndexShift)) ? "[X] SHIFT" : "[ ] SHIFT",
      ModIndexShift, ducky_gen_scene_modifier_builder_submenu_callback, app);
  submenu_add_item(
      app->submenu, (mask & (1 << ModIndexGui)) ? "[X] GUI" : "[ ] GUI",
      ModIndexGui, ducky_gen_scene_modifier_builder_submenu_callback, app);
  submenu_add_item(app->submenu,
                   (mask & (1 << ModIndexDelete)) ? "[X] DELETE" : "[ ] DELETE",
                   ModIndexDelete,
                   ducky_gen_scene_modifier_builder_submenu_callback, app);

  submenu_add_item(app->submenu, "Next: Select Key", ModIndexNext,
                   ducky_gen_scene_modifier_builder_submenu_callback, app);
}

void ducky_gen_scene_modifier_builder_on_enter(void *context) {
  DuckyGenApp *app = context;


  if (app->is_building_modifier) {

    scene_manager_set_scene_state(app->scene_manager,
                                  DuckyGenSceneModifierBuilder, 0);
    app->modifier_str[0] = '\0';
    app->is_building_modifier = false;
  }
  builder_update_submenu(app);
  view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewSubmenu);
}

bool ducky_gen_scene_modifier_builder_on_event(void *context,
                                               SceneManagerEvent event) {
  UNUSED(context);
  UNUSED(event);
  return false;
}

void ducky_gen_scene_modifier_builder_on_exit(void *context) {
  UNUSED(context);
}
