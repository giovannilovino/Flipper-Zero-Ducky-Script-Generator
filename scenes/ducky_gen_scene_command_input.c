#include "../ducky_gen_app_i.h"

void ducky_gen_scene_command_input_callback(void* context) {
    DuckyGenApp* app = context;
    if(app->is_editing) {
        ScriptLine* line = ducky_script_get_line(
            editor_view_get_script(app->editor_view), app->focused_line_index);
        line->type = app->pending_line_type;
        if(line->parameter) free(line->parameter);
        line->parameter = strdup(app->text_input_buffer);
    } else {


        if(app->focused_line_index <
           (uint32_t)ducky_script_get_count(editor_view_get_script(app->editor_view))) {
            ducky_script_insert_line(
                editor_view_get_script(app->editor_view),
                app->focused_line_index,
                app->pending_line_type,
                app->text_input_buffer);
        } else {
            ducky_script_add_line(
                editor_view_get_script(app->editor_view),
                app->pending_line_type,
                app->text_input_buffer);
        }
    }

    scene_manager_search_and_switch_to_previous_scene(app->scene_manager, DuckyGenSceneEditor);
}

void ducky_gen_scene_command_input_on_enter(void* context) {
    DuckyGenApp* app = context;











    text_input_reset(app->text_input);

    const char* header = "Enter Text";
    if(app->pending_line_type == ScriptLineTypeDelay)
        header = "Delay (ms)";
    else if(app->pending_line_type == ScriptLineTypeRem)
        header = "Comment";
    else if(app->pending_line_type == ScriptLineTypeRepeat)
        header = "Repeat Count";

    text_input_set_header_text(app->text_input, header);
    memset(app->text_input_buffer, 0, sizeof(app->text_input_buffer));

    text_input_set_result_callback(
        app->text_input,
        ducky_gen_scene_command_input_callback,
        app,
        app->text_input_buffer,
        sizeof(app->text_input_buffer),
        true);

    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewTextInput);
}

bool ducky_gen_scene_command_input_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void ducky_gen_scene_command_input_on_exit(void* context) {
    UNUSED(context);
}
