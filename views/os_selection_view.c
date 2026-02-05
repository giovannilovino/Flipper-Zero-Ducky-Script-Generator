#include "os_selection_view.h"
#include <furi.h>
#include <gui/elements.h>
#include <gui/icon_i.h>

struct OsSelectionView {
    View* view;
    OsSelectionViewCallback callback;
    void* callback_context;
};

typedef struct {
    uint8_t selected_index;
} OsSelectionViewModel;

#define MENU_COUNT 7
static const char* menu_items[MENU_COUNT] =
    {"New: Windows", "New: Linux", "New: macOS", "New: Android", "Load Script", "About", "Quit"};

static void os_selection_view_draw_callback(Canvas* canvas, void* _model) {
    OsSelectionViewModel* model = _model;

    canvas_clear(canvas);


    canvas_set_color(canvas, ColorBlack);
    canvas_draw_box(canvas, 0, 0, 128, 12);

    canvas_set_color(canvas, ColorWhite);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 6, AlignCenter, AlignCenter, "Ducky Generator");

    canvas_set_color(canvas, ColorBlack);



    int dx = 5;
    int dy = 25;


    canvas_draw_rframe(canvas, dx, dy + 15, 25, 12, 3);
    canvas_draw_circle(canvas, dx + 8, dy + 8, 7);


    canvas_draw_box(canvas, dx + 14, dy + 8, 6, 4);


    canvas_draw_disc(canvas, dx + 6, dy + 6, 2);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_dot(canvas, dx + 6, dy + 6);
    canvas_set_color(canvas, ColorBlack);


    canvas_draw_line(canvas, dx + 10, dy + 20, dx + 18, dy + 20);


    int bx = 35;
    int by = 18;
    int bw = 90;
    int bh = 30;


    canvas_draw_line(canvas, dx + 15, dy + 5, bx, by + 10);
    canvas_draw_line(canvas, dx + 15, dy + 5, bx + 10, by + 15);


    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, bx, by, bw, bh);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, bx, by, bw, bh, 4);


    const char* current_item = menu_items[model->selected_index];

    canvas_set_font(
        canvas,
        FontPrimary);
    canvas_draw_str_aligned(
        canvas, bx + bw / 2, by + bh / 2 - 2, AlignCenter, AlignCenter, current_item);


    canvas_draw_str_aligned(canvas, bx + 5, by + bh / 2 - 2, AlignLeft, AlignCenter, "<");
    canvas_draw_str_aligned(canvas, bx + bw - 5, by + bh / 2 - 2, AlignRight, AlignCenter, ">");


    int btn_w = 70;
    int btn_h = 11;
    int btn_x = 64 - btn_w / 2;
    int btn_y = 64 - btn_h - 1;

    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rbox(canvas, btn_x, btn_y, btn_w, btn_h, 3);

    canvas_set_color(canvas, ColorWhite);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(
        canvas, 64, btn_y + btn_h / 2, AlignCenter, AlignCenter, "Select (OK)");

    canvas_set_color(canvas, ColorBlack);
}

static bool os_selection_view_input_callback(InputEvent* event, void* context) {
    OsSelectionView* instance = context;
    if(event->type == InputTypeShort ||
       event->type == InputTypeRepeat) {
        if(event->key == InputKeyLeft) {
            with_view_model(
                instance->view,
                OsSelectionViewModel * model,
                {
                    if(model->selected_index > 0) {
                        model->selected_index--;
                    } else {
                        model->selected_index = MENU_COUNT - 1;
                    }
                },
                true);
            return true;
        } else if(event->key == InputKeyRight) {
            with_view_model(
                instance->view,
                OsSelectionViewModel * model,
                {
                    if(model->selected_index < MENU_COUNT - 1) {
                        model->selected_index++;
                    } else {
                        model->selected_index = 0;
                    }
                },
                true);
            return true;
        } else if(event->key == InputKeyOk) {
            uint8_t selected_index = 0;
            with_view_model(
                instance->view,
                OsSelectionViewModel * model,
                { selected_index = model->selected_index; },
                false);

            if(instance->callback) {
                instance->callback(instance->callback_context, selected_index);
            }
            return true;
        } else if(event->key == InputKeyBack && event->type == InputTypeLong) {

            if(instance->callback) {
                instance->callback(instance->callback_context, MENU_COUNT - 1);
            }
            return true;
        }
    }
    return false;
}

OsSelectionView* os_selection_view_alloc() {
    OsSelectionView* instance = malloc(sizeof(OsSelectionView));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(OsSelectionViewModel));
    view_set_context(instance->view, instance);
    view_set_draw_callback(instance->view, os_selection_view_draw_callback);
    view_set_input_callback(instance->view, os_selection_view_input_callback);

    with_view_model(
        instance->view, OsSelectionViewModel * model, { model->selected_index = 0; }, true);

    return instance;
}

void os_selection_view_free(OsSelectionView* instance) {
    furi_assert(instance);
    view_free(instance->view);
    free(instance);
}

View* os_selection_view_get_view(OsSelectionView* instance) {
    furi_assert(instance);
    return instance->view;
}

void os_selection_view_set_callback(
    OsSelectionView* instance,
    OsSelectionViewCallback callback,
    void* context) {
    furi_assert(instance);
    instance->callback = callback;
    instance->callback_context = context;
}
