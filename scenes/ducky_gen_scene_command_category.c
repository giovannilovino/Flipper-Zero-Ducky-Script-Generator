#include "../ducky_gen_app_i.h"


enum {
    SubmenuIndexInputDelay,
    SubmenuIndexActionKeys,
    SubmenuIndexNavigation,
    SubmenuIndexFunctionKeys,
    SubmenuIndexModifiers,
    SubmenuIndexAdvanced,
};

void ducky_gen_scene_command_category_submenu_callback(void* context, uint32_t index) {
    DuckyGenApp* app = context;


    if(index == ScriptLineTypeString || index == ScriptLineTypeStringLn ||
       index == ScriptLineTypeRem || index == ScriptLineTypeDelay ||
       index == ScriptLineTypeRepeat) {
        app->pending_line_type = index;
        scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandInput);
    } else {

        DuckyScript* script = editor_view_get_script(app->editor_view);
        if(app->is_editing) {
            ScriptLine* line = ducky_script_get_line(script, app->focused_line_index);
            line->type = index;
            if(line->parameter) {
                free(line->parameter);
                line->parameter = NULL;
            }
        } else {
            if(app->focused_line_index < ducky_script_get_count(script)) {
                ducky_script_insert_line(script, app->focused_line_index, index, NULL);
            } else {
                ducky_script_add_line(script, index, NULL);
            }
        }
        scene_manager_search_and_switch_to_previous_scene(app->scene_manager, DuckyGenSceneEditor);
    }
}

void ducky_gen_scene_command_category_on_enter(void* context) {
    DuckyGenApp* app = context;
    submenu_reset(app->submenu);

    uint32_t category =
        scene_manager_get_scene_state(app->scene_manager, DuckyGenSceneCommandType);

    if(category == SubmenuIndexActionKeys) {
        submenu_set_header(app->submenu, "Action Keys");
        submenu_add_item(
            app->submenu,
            "ENTER",
            ScriptLineTypeEnter,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Space",
            ScriptLineTypeSpace,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Tab",
            ScriptLineTypeTab,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "ESC",
            ScriptLineTypeEsc,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Backspace",
            ScriptLineTypeBackspace,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Delete",
            ScriptLineTypeDelete,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Insert",
            ScriptLineTypeInsert,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Caps Lock",
            ScriptLineTypeCapsLock,
            ducky_gen_scene_command_category_submenu_callback,
            app);

    } else if(category == SubmenuIndexNavigation) {
        submenu_set_header(app->submenu, "Navigation");
        submenu_add_item(
            app->submenu,
            "UP Arrow",
            ScriptLineTypeUp,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "DOWN Arrow",
            ScriptLineTypeDown,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "LEFT Arrow",
            ScriptLineTypeLeft,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "RIGHT Arrow",
            ScriptLineTypeRight,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Home",
            ScriptLineTypeHome,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "End",
            ScriptLineTypeEnd,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Page Up",
            ScriptLineTypePageUp,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "Page Down",
            ScriptLineTypePageDown,
            ducky_gen_scene_command_category_submenu_callback,
            app);
    } else if(category == SubmenuIndexFunctionKeys) {
        submenu_set_header(app->submenu, "Function Keys");
        for(int i = 0; i < 12; i++) {
            char buf[8];
            snprintf(buf, sizeof(buf), "F%d", i + 1);
            submenu_add_item(
                app->submenu,
                buf,
                ScriptLineTypeF1 + i,
                ducky_gen_scene_command_category_submenu_callback,
                app);
        }
    } else if(category == SubmenuIndexInputDelay) {

        submenu_set_header(app->submenu, "Input & Delay");
        submenu_add_item(
            app->submenu,
            "STRING (Text)",
            ScriptLineTypeString,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "STRINGLN (Line)",
            ScriptLineTypeStringLn,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "REM (Comment)",
            ScriptLineTypeRem,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "DELAY (Wait)",
            ScriptLineTypeDelay,
            ducky_gen_scene_command_category_submenu_callback,
            app);
    } else if(category == SubmenuIndexAdvanced) {
        submenu_set_header(app->submenu, "Advanced");
        submenu_add_item(
            app->submenu,
            "REPEAT (Loop)",
            ScriptLineTypeRepeat,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "HOLD (Press)",
            ScriptLineTypeHold,
            ducky_gen_scene_command_category_submenu_callback,
            app);
        submenu_add_item(
            app->submenu,
            "RELEASE (Release)",
            ScriptLineTypeRelease,
            ducky_gen_scene_command_category_submenu_callback,
            app);
    }

    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewSubmenu);
}

bool ducky_gen_scene_command_category_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void ducky_gen_scene_command_category_on_exit(void* context) {
    UNUSED(context);
}
