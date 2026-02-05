#include "../ducky_gen_app_i.h"

void ducky_gen_scene_preview_on_enter(void* context) {
    DuckyGenApp* app = context;
    Widget* widget = app->widget;

    widget_reset(widget);

    DuckyScript* script = editor_view_get_script(app->editor_view);
    FuriString* full_script = furi_string_alloc();

    ScriptLine* current = script->head;
    while(current) {
        if(current->type == ScriptLineTypeCombo) {
            if(current->parameter) furi_string_cat(full_script, current->parameter);
        } else {
            furi_string_cat_printf(full_script, "%s", ducky_script_get_type_str(current->type));
            if(current->parameter) {
                furi_string_cat_printf(full_script, " %s", current->parameter);
            }
        }
        furi_string_cat(full_script, "\n");
        current = current->next;
    }

    widget_add_text_scroll_element(widget, 0, 0, 128, 64, furi_string_get_cstr(full_script));

    furi_string_free(full_script);

    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewWidget);
}

bool ducky_gen_scene_preview_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void ducky_gen_scene_preview_on_exit(void* context) {
    DuckyGenApp* app = context;
    widget_reset(app->widget);
}
