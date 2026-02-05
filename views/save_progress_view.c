#include "save_progress_view.h"
#include <furi.h>
#include <gui/elements.h>

typedef struct {
    float progress;
    bool success;
    int8_t selected_btn;
} SaveProgressViewModel;

struct SaveProgressView {
    View* view;
    SaveProgressViewCallback callback;
    void* callback_context;
};

static void save_progress_view_draw_callback(Canvas* canvas, void* _model) {
    SaveProgressViewModel* model = _model;

    canvas_clear(canvas);


    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rbox(canvas, 0, 0, 128, 14, 3);
    canvas_set_color(canvas, ColorWhite);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 7, AlignCenter, AlignCenter, "Ducky Generator");

    canvas_set_color(canvas, ColorBlack);

    if(model->success) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 64, 22, AlignCenter, AlignBottom, "Saved Success!");

        const char* buttons[] = {"Quick Run", "Back Editor", "Quit"};
        for(int i = 0; i < 3; i++) {
            int y = 28 + (i * 12);
            if(i == model->selected_btn) {

                canvas_set_color(canvas, ColorBlack);
                canvas_draw_rbox(canvas, 18, y, 92, 11, 2);
                canvas_set_color(canvas, ColorWhite);
                canvas_draw_rbox(canvas, 20, y + 1, 88, 9, 1);
                canvas_set_color(canvas, ColorBlack);
            } else {

                canvas_set_color(canvas, ColorBlack);
                canvas_draw_rbox(canvas, 24, y, 80, 11, 2);
                canvas_set_color(canvas, ColorWhite);
            }
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str_aligned(canvas, 64, y + 6, AlignCenter, AlignCenter, buttons[i]);
        }
    } else {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 64, 30, AlignCenter, AlignCenter, "Saving...");

        int bar_x = 14;
        int bar_y = 40;
        int bar_w = 100;
        int bar_h = 10;

        canvas_draw_rframe(canvas, bar_x, bar_y, bar_w, bar_h, 3);
        int fill_w = (int)(model->progress * (bar_w - 4));
        if(fill_w > 0) {
            canvas_draw_rbox(canvas, bar_x + 2, bar_y + 2, fill_w, bar_h - 4, 2);
        }

        char percent_buf[16];
        snprintf(percent_buf, sizeof(percent_buf), "%d%%", (int)(model->progress * 100));
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 58, AlignCenter, AlignCenter, percent_buf);
    }
}

static bool save_progress_view_input_callback(InputEvent* event, void* context) {
    SaveProgressView* instance = context;
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        bool success = false;
        with_view_model(
            instance->view, SaveProgressViewModel * model, { success = model->success; }, false);
        if(!success) return false;

        if(event->key == InputKeyUp) {
            with_view_model(
                instance->view,
                SaveProgressViewModel * model,
                {
                    if(model->selected_btn > 0)
                        model->selected_btn--;
                    else
                        model->selected_btn = 2;
                },
                true);
            return true;
        } else if(event->key == InputKeyDown) {
            with_view_model(
                instance->view,
                SaveProgressViewModel * model,
                {
                    if(model->selected_btn < 2)
                        model->selected_btn++;
                    else
                        model->selected_btn = 0;
                },
                true);
            return true;
        } else if(event->key == InputKeyOk) {
            int8_t selected = 0;
            with_view_model(
                instance->view,
                SaveProgressViewModel * model,
                { selected = model->selected_btn; },
                false);
            if(instance->callback) {
                instance->callback(instance->callback_context, selected);
            }
            return true;
        }
    }
    return false;
}

SaveProgressView* save_progress_view_alloc() {
    SaveProgressView* instance = malloc(sizeof(SaveProgressView));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(SaveProgressViewModel));
    view_set_draw_callback(instance->view, save_progress_view_draw_callback);
    view_set_input_callback(instance->view, save_progress_view_input_callback);
    view_set_context(instance->view, instance);

    with_view_model(
        instance->view,
        SaveProgressViewModel * model,
        {
            model->progress = 0;
            model->success = false;
            model->selected_btn = 0;
        },
        true);

    return instance;
}

void save_progress_view_free(SaveProgressView* instance) {
    furi_assert(instance);
    view_free(instance->view);
    free(instance);
}

View* save_progress_view_get_view(SaveProgressView* instance) {
    furi_assert(instance);
    return instance->view;
}

void save_progress_view_set_progress(SaveProgressView* instance, float progress) {
    furi_assert(instance);
    with_view_model(
        instance->view, SaveProgressViewModel * model, { model->progress = progress; }, true);
}

void save_progress_view_set_success(SaveProgressView* instance, bool success) {
    furi_assert(instance);
    with_view_model(
        instance->view, SaveProgressViewModel * model, { model->success = success; }, true);
}

void save_progress_view_set_callback(
    SaveProgressView* instance,
    SaveProgressViewCallback callback,
    void* context) {
    furi_assert(instance);
    instance->callback = callback;
    instance->callback_context = context;
}
