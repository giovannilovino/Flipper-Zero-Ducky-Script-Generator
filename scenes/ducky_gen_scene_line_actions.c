#include "../ducky_gen_app_i.h"

enum {
    LineActionEdit,
    LineActionDelete,
    LineActionMoveUp,
    LineActionMoveDown,
    LineActionInsertAbove,
    LineActionInsertBelow,
    LineActionDuplicate,
};

void ducky_gen_scene_line_actions_submenu_callback(void* context, uint32_t index) {
    DuckyGenApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void ducky_gen_scene_line_actions_on_enter(void* context) {
    DuckyGenApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_reset(submenu);
    submenu_set_header(submenu, "Line Actions");

    submenu_add_item(
        submenu, "Edit", LineActionEdit, ducky_gen_scene_line_actions_submenu_callback, app);
    submenu_add_item(
        submenu, "Delete", LineActionDelete, ducky_gen_scene_line_actions_submenu_callback, app);
    submenu_add_item(
        submenu, "Move Up", LineActionMoveUp, ducky_gen_scene_line_actions_submenu_callback, app);
    submenu_add_item(
        submenu,
        "Move Down",
        LineActionMoveDown,
        ducky_gen_scene_line_actions_submenu_callback,
        app);
    submenu_add_item(
        submenu,
        "Insert Above",
        LineActionInsertAbove,
        ducky_gen_scene_line_actions_submenu_callback,
        app);
    submenu_add_item(
        submenu,
        "Insert Below",
        LineActionInsertBelow,
        ducky_gen_scene_line_actions_submenu_callback,
        app);
    submenu_add_item(
        submenu,
        "Duplicate",
        LineActionDuplicate,
        ducky_gen_scene_line_actions_submenu_callback,
        app);

    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewSubmenu);
}

bool ducky_gen_scene_line_actions_on_event(void* context, SceneManagerEvent event) {
    DuckyGenApp* app = context;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == LineActionEdit) {
            app->is_editing = true;
            ScriptLine* line = ducky_script_get_line(
                editor_view_get_script(app->editor_view), app->focused_line_index);
            if(line->parameter) {
                strlcpy(app->text_input_buffer, line->parameter, sizeof(app->text_input_buffer));
            } else {
                app->text_input_buffer[0] = '\0';
            }
            app->pending_line_type = line->type;


            if(line->type == ScriptLineTypeString || line->type == ScriptLineTypeStringLn ||
               line->type == ScriptLineTypeDelay || line->type == ScriptLineTypeRem ||
               line->type == ScriptLineTypeCombo) {
                scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandInput);
            } else {


                scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandType);
            }
        } else if(event.event == LineActionDelete) {
            ducky_script_delete_line(
                editor_view_get_script(app->editor_view), app->focused_line_index);
            scene_manager_previous_scene(app->scene_manager);
        } else if(event.event == LineActionMoveUp) {
            ducky_script_move_line_up(
                editor_view_get_script(app->editor_view), app->focused_line_index);
            scene_manager_previous_scene(app->scene_manager);
        } else if(event.event == LineActionMoveDown) {
            ducky_script_move_line_down(
                editor_view_get_script(app->editor_view), app->focused_line_index);
            scene_manager_previous_scene(app->scene_manager);
        } else if(event.event == LineActionInsertAbove) {
            app->is_editing = false;


            scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandType);
        } else if(event.event == LineActionInsertBelow) {
            app->is_editing = false;
            app->focused_line_index++;
            scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandType);
        } else if(event.event == LineActionDuplicate) {
            ScriptLine* line = ducky_script_get_line(
                editor_view_get_script(app->editor_view), app->focused_line_index);
            ducky_script_insert_line(
                editor_view_get_script(app->editor_view),
                app->focused_line_index + 1,
                line->type,
                line->parameter);
            scene_manager_previous_scene(app->scene_manager);
        }
        return true;
    }
    return false;
}

void ducky_gen_scene_line_actions_on_exit(void* context) {
    DuckyGenApp* app = context;
    submenu_reset(app->submenu);
}
