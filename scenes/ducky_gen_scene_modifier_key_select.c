#include "../ducky_gen_app_i.h"

void ducky_gen_scene_modifier_key_select_submenu_callback(void *context,
                                                          uint32_t index) {
  DuckyGenApp *app = context;






  char final_combo[128];
  strlcpy(final_combo, app->modifier_str, sizeof(final_combo));





  if (index < 26) {

    char c = 'a' + index;
    char buf[2] = {c, 0};
    strlcat(final_combo, buf, sizeof(final_combo));
  } else {


    int special = index - 26;
    if (special == 0)
      strlcat(final_combo, "ENTER", sizeof(final_combo));
    else if (special == 1)
      strlcat(final_combo, "ESC", sizeof(final_combo));
    else if (special == 2)
      strlcat(final_combo, "TAB", sizeof(final_combo));
    else if (special == 3)
      strlcat(final_combo, "SPACE", sizeof(final_combo));
    else if (special == 4)
      strlcat(final_combo, "F1", sizeof(final_combo));
    else if (special == 5)
      strlcat(final_combo, "DELETE", sizeof(final_combo));

  }







  ducky_script_add_line(editor_view_get_script(app->editor_view),
                        ScriptLineTypeCombo, final_combo);


  scene_manager_search_and_switch_to_previous_scene(app->scene_manager,
                                                    DuckyGenSceneEditor);
}

void ducky_gen_scene_modifier_key_select_on_enter(void *context) {
  DuckyGenApp *app = context;
  submenu_reset(app->submenu);
  submenu_set_header(app->submenu, "Select Final Key");


  for (int i = 0; i < 26; i++) {
    char buf[2] = {'a' + i, 0};
    submenu_add_item(app->submenu, buf, i,
                     ducky_gen_scene_modifier_key_select_submenu_callback, app);
  }

  int base = 26;
  submenu_add_item(app->submenu, "ENTER", base++,
                   ducky_gen_scene_modifier_key_select_submenu_callback, app);
  submenu_add_item(app->submenu, "ESC", base++,
                   ducky_gen_scene_modifier_key_select_submenu_callback, app);
  submenu_add_item(app->submenu, "TAB", base++,
                   ducky_gen_scene_modifier_key_select_submenu_callback, app);
  submenu_add_item(app->submenu, "SPACE", base++,
                   ducky_gen_scene_modifier_key_select_submenu_callback, app);
  submenu_add_item(app->submenu, "F1", base++,
                   ducky_gen_scene_modifier_key_select_submenu_callback, app);
  submenu_add_item(app->submenu, "F2", base++,
                   ducky_gen_scene_modifier_key_select_submenu_callback, app);
  submenu_add_item(app->submenu, "DELETE", base++,
                   ducky_gen_scene_modifier_key_select_submenu_callback, app);


  view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewSubmenu);
}

bool ducky_gen_scene_modifier_key_select_on_event(void *context,
                                                  SceneManagerEvent event) {
  UNUSED(context);
  UNUSED(event);
  return false;
}

void ducky_gen_scene_modifier_key_select_on_exit(void *context) {
  UNUSED(context);
}
