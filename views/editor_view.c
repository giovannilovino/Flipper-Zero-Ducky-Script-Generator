#include "editor_view.h"
#include "../helpers/ducky_script.h"
#include <furi.h>
#include <gui/elements.h>
#include <gui/view_i.h>

struct EditorView {
    View* view;
    ViewDispatcher* view_dispatcher;
    DuckyScript* script;
};

typedef struct {
    DuckyScript* script;
    int32_t selected_index;
    int32_t scroll_offset;
    int32_t horizontal_offset;
    char os_name[32];
    bool popup_visible;
    int8_t popup_index;
} EditorViewModel;

static void editor_view_draw_callback(Canvas* canvas, void* _model) {
    EditorViewModel* model = _model;

    canvas_clear(canvas);


    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 1, 8, "Edit Payload");


    char os_buf[64];
    snprintf(os_buf, sizeof(os_buf), "[%s]", model->os_name);
    canvas_draw_str_aligned(canvas, 127, 8, AlignRight, AlignBottom, os_buf);

    canvas_draw_line(canvas, 0, 10, 128, 10);






    int list_y = 12;
    int list_h = 40;
    int list_w = 126;
    int list_x = 1;

    canvas_draw_rframe(canvas, list_x, list_y, list_w, list_h, 3);


    canvas_set_font(canvas, FontSecondary);

    int32_t item_height = 9;
    int32_t visible_items = (list_h - 2) / item_height;
    int32_t content_x = list_x + 3;
    int32_t content_y_base = list_y + 2 + 7;

    if(!model->script || model->script->count == 0) {
        canvas_draw_str_aligned(
            canvas, 64, list_y + list_h / 2, AlignCenter, AlignCenter, "Empty - Press OK");
    } else {

        if(model->selected_index < model->scroll_offset) {
            model->scroll_offset = model->selected_index;
        } else if(model->selected_index >= model->scroll_offset + visible_items) {
            model->scroll_offset = model->selected_index - visible_items + 1;
        }

        for(int32_t i = 0; i < visible_items; i++) {
            int32_t index = model->scroll_offset + i;
            if(index > (int32_t)model->script->count) break;

            int32_t y = content_y_base + (i * item_height);


            if(index == model->selected_index) {
                canvas_set_color(canvas, ColorBlack);
                canvas_draw_box(canvas, list_x + 1, y - 7, list_w - 2, item_height);
                canvas_set_color(canvas, ColorWhite);
            } else {
                canvas_set_color(canvas, ColorBlack);
            }

            if(index == (int32_t)model->script->count) {

                canvas_draw_str(canvas, content_x - model->horizontal_offset, y, "[Add New...]");
                continue;
            }

            ScriptLine* line = ducky_script_get_line(model->script, index);
            if(!line) continue;

            char buffer[64];
            if(line->parameter) {
                snprintf(
                    buffer,
                    sizeof(buffer),
                    "%s %s",
                    ducky_script_get_type_str(line->type),
                    line->parameter);
            } else {
                snprintf(buffer, sizeof(buffer), "%s", ducky_script_get_type_str(line->type));
            }
            canvas_draw_str(canvas, content_x - model->horizontal_offset, y, buffer);
        }
    }
    canvas_set_color(canvas, ColorBlack);


    if(model->script && model->script->count > (uint32_t)(visible_items - 1)) {


        int scroll_y = list_y + 2;
        int scroll_h = list_h - 4;

        float progress = 0;
        if(model->script->count > 0) {
            progress = (float)model->selected_index / (float)(model->script->count);
        }
        int indicator_y = scroll_y + (int)(progress * (scroll_h - 4));

        canvas_draw_line(canvas, 125, scroll_y, 125, scroll_y + scroll_h);
        canvas_draw_box(canvas, 124, indicator_y, 3, 4);
    }







    int btn_y = 54;
    int btn_h = 10;


    canvas_draw_rframe(canvas, 0, btn_y, 30, btn_h, 2);
    canvas_draw_box(canvas, 0, btn_y, 30, btn_h);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_str_aligned(canvas, 15, btn_y + btn_h / 2 + 1, AlignCenter, AlignCenter, "Long >");
    canvas_set_color(canvas, ColorBlack);


    canvas_draw_rbox(canvas, 49, btn_y, 30, btn_h, 2);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_str_aligned(canvas, 64, btn_y + btn_h / 2 + 1, AlignCenter, AlignCenter, "OK");
    canvas_set_color(canvas, ColorBlack);


    canvas_draw_rbox(canvas, 98, btn_y, 30, btn_h, 2);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_str_aligned(canvas, 113, btn_y + btn_h / 2 + 1, AlignCenter, AlignCenter, "< >");
    canvas_set_color(canvas, ColorBlack);

    if(model->popup_visible) {

        canvas_set_color(canvas, ColorWhite);
        canvas_draw_rbox(canvas, 25, 5, 78, 54, 3);
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_rframe(canvas, 25, 5, 78, 54, 3);

        const char* popup_items[] = {
            "Edit", "Delete", "Duplicate", "Move Up", "Move Down", "Insert Above", "Insert Below"};

        canvas_set_font(canvas, FontSecondary);
        for(int i = 0; i < 7; i++) {
            int y = 13 + (i * 7);
            if(i == model->popup_index) {

                canvas_draw_rframe(canvas, 27, y - 6, 74, 8, 2);
                canvas_draw_str(canvas, 32, y, popup_items[i]);
            } else {
                canvas_draw_str(canvas, 32, y, popup_items[i]);
            }
        }
    }
}

static bool editor_view_input_callback(InputEvent* event, void* context) {
    EditorView* instance = context;
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        if(event->key == InputKeyUp) {
            bool handled = false;
            with_view_model(
                instance->view,
                EditorViewModel * model,
                {
                    if(model->popup_visible) {
                        if(model->popup_index > 0)
                            model->popup_index--;
                        else
                            model->popup_index = 6;
                        handled = true;
                    } else {
                        if(model->selected_index > 0) {
                            model->selected_index--;
                        }
                    }
                },
                true);
            if(handled) return true;
        } else if(event->key == InputKeyDown) {
            bool handled = false;
            with_view_model(
                instance->view,
                EditorViewModel * model,
                {
                    if(model->popup_visible) {
                        if(model->popup_index < 6)
                            model->popup_index++;
                        else
                            model->popup_index = 0;
                        handled = true;
                    } else {
                        if(model->script &&
                           model->selected_index < (int32_t)model->script->count) {
                            model->selected_index++;
                        }
                    }
                },
                true);
            if(handled) return true;
        } else if(event->key == InputKeyLeft) {
            with_view_model(
                instance->view,
                EditorViewModel * model,
                {
                    if(!model->popup_visible && model->horizontal_offset > 0) {
                        model->horizontal_offset -= 10;
                    }
                },
                true);
            return true;
        } else if(event->key == InputKeyRight) {
            with_view_model(
                instance->view,
                EditorViewModel * model,
                {
                    if(!model->popup_visible) {
                        model->horizontal_offset += 10;
                    }
                },
                true);
            return true;
        } else if(event->key == InputKeyOk) {
            with_view_model(
                instance->view,
                EditorViewModel * model,
                {
                    if(model->popup_visible) {
                        model->popup_visible = false;
                        view_dispatcher_send_custom_event(
                            instance->view_dispatcher, 100 + model->popup_index);
                    } else {
                        if((uint32_t)model->selected_index < model->script->count) {
                            model->popup_visible = true;
                            model->popup_index = 0;
                        } else {
                            view_dispatcher_send_custom_event(instance->view_dispatcher, 0);
                        }
                    }
                },
                true);
            return true;
        } else if(event->key == InputKeyBack) {
            bool handled = false;
            with_view_model(
                instance->view,
                EditorViewModel * model,
                {
                    if(model->popup_visible) {
                        model->popup_visible = false;
                        handled = true;
                    }
                },
                true);
            if(handled) return true;
        }
    } else if(event->type == InputTypeLong) {
        if(event->key == InputKeyRight) {
            view_dispatcher_send_custom_event(instance->view_dispatcher, 10);
            return true;
        } else if(event->key == InputKeyLeft) {
            view_dispatcher_send_custom_event(instance->view_dispatcher, 20);
            return true;
        } else if(event->key == InputKeyOk) {
            view_dispatcher_send_custom_event(
                instance->view_dispatcher, 30);
            return true;
        }
    }
    return false;
}

EditorView* editor_view_alloc(ViewDispatcher* view_dispatcher) {
    EditorView* instance = malloc(sizeof(EditorView));
    instance->view = view_alloc();
    instance->view_dispatcher = view_dispatcher;
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(EditorViewModel));

    view_set_context(instance->view, instance);
    view_set_draw_callback(instance->view, editor_view_draw_callback);
    view_set_input_callback(instance->view, editor_view_input_callback);

    instance->script = ducky_script_alloc();

    with_view_model(
        instance->view,
        EditorViewModel * model,
        {
            model->script = instance->script;
            model->selected_index = 0;
            model->scroll_offset = 0;
            model->popup_visible = false;
            model->popup_index = 0;
            strncpy(model->os_name, "Unknown", sizeof(model->os_name));
        },
        true);

    return instance;
}

void editor_view_free(EditorView* instance) {
    furi_assert(instance);
    ducky_script_free(instance->script);
    view_free(instance->view);
    free(instance);
}

View* editor_view_get_view(EditorView* instance) {
    furi_assert(instance);
    return instance->view;
}

void editor_view_set_os_name(EditorView* instance, const char* os_name) {
    furi_assert(instance);
    with_view_model(
        instance->view,
        EditorViewModel * model,
        { strncpy(model->os_name, os_name, sizeof(model->os_name)); },
        true);
}

DuckyScript* editor_view_get_script(EditorView* instance) {
    furi_assert(instance);
    return instance->script;
}

void editor_view_set_script(EditorView* instance, DuckyScript* script) {
    furi_assert(instance);
    if(instance->script) {
        ducky_script_free(instance->script);
    }
    instance->script = script;

    with_view_model(
        instance->view,
        EditorViewModel * model,
        {
            model->script = instance->script;
            model->selected_index = 0;
            model->scroll_offset = 0;
        },
        true);
}

int32_t editor_view_get_selected_index(EditorView* instance) {
    furi_assert(instance);
    int32_t index = 0;
    with_view_model(
        instance->view, EditorViewModel * model, { index = model->selected_index; }, false);
    return index;
}
