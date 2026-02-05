#include "../ducky_gen_app_i.h"

void ducky_gen_scene_editor_on_enter(void* context) {
    DuckyGenApp* app = context;
    editor_view_set_os_name(app->editor_view, app->os_name);
    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewEditor);
}

bool ducky_gen_scene_editor_on_event(void* context, SceneManagerEvent event) {
    DuckyGenApp* app = context;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == 0) {
            app->is_editing = false;
            app->focused_line_index =
                ducky_script_get_count(editor_view_get_script(app->editor_view));
            scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandType);
            return true;
        } else if(event.event >= 100 && event.event <= 106) {
            uint32_t action_idx = event.event - 100;
            uint32_t line_idx = (uint32_t)editor_view_get_selected_index(app->editor_view);
            DuckyScript* script = editor_view_get_script(app->editor_view);

            if(action_idx == 0) {
                app->focused_line_index = line_idx;
                app->is_editing = true;
                scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandType);
            } else if(action_idx == 1) {
                ducky_script_delete_line(script, line_idx);
            } else if(action_idx == 2) {
                ScriptLine* line = ducky_script_get_line(script, line_idx);
                ducky_script_insert_line(script, line_idx + 1, line->type, line->parameter);
            } else if(action_idx == 3) {
                ducky_script_move_line_up(script, line_idx);
            } else if(action_idx == 4) {
                ducky_script_move_line_down(script, line_idx);
            } else if(action_idx == 5) {
                app->focused_line_index = line_idx;
                app->is_editing = false;
                scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandType);

            } else if(action_idx == 6) {
                app->focused_line_index = line_idx + 1;
                app->is_editing = false;
                scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandType);
            }
            return true;
        } else if(event.event == 10) {

            scene_manager_next_scene(app->scene_manager, DuckyGenSceneSaveFilename);
            return true;
        } else if(event.event == 20) {
            scene_manager_next_scene(app->scene_manager, DuckyGenScenePreview);
            return true;
        } else if(event.event == 30) {
            scene_manager_next_scene(app->scene_manager, DuckyGenSceneSaveFilename);
            return true;
        }
    }
    return false;
}

void ducky_gen_scene_editor_on_exit(void* context) {
    UNUSED(context);
}
