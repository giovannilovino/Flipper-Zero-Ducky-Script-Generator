#include "../ducky_gen_app_i.h"
#include "../helpers/ducky_storage.h"

static const char *os_names_lookup[] = {"Windows", "Linux", "Android", "macOS"};

void ducky_gen_scene_os_selection_callback(void *context, uint32_t index) {
  DuckyGenApp *app = context;
  if (index < 4) {

    strncpy(app->os_name, os_names_lookup[index], sizeof(app->os_name));


    if (ducky_gen_check_and_create_dir(app->storage, app->os_name)) {
      FURI_LOG_I("DuckyGen", "Directory checked/created for %s", app->os_name);
    } else {
      FURI_LOG_E("DuckyGen", "Failed to create directory for %s", app->os_name);

    }

    scene_manager_next_scene(app->scene_manager, DuckyGenSceneEditor);
  }
}

void ducky_gen_scene_os_selection_on_enter(void *context) {
  DuckyGenApp *app = context;
  os_selection_view_set_callback(app->os_selection_view,
                                 ducky_gen_scene_os_selection_callback, app);
  view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewOsSelection);
}

bool ducky_gen_scene_os_selection_on_event(void *context,
                                           SceneManagerEvent event) {
  UNUSED(context);
  UNUSED(event);
  return false;
}

void ducky_gen_scene_os_selection_on_exit(void *context) { UNUSED(context); }
