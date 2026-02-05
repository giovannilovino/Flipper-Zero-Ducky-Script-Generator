#include "splash_view.h"
#include <furi.h>
#include <gui/elements.h>

struct SplashView {
  View *view;
  int animation_phase;
};

typedef struct {
  int animation_phase;
} SplashViewModel;

static void splash_view_draw_callback(Canvas *canvas, void *_model) {
  SplashViewModel *model = _model;

  canvas_clear(canvas);


  canvas_set_font(canvas, FontSecondary);
  canvas_draw_str_aligned(canvas, 64, 25, AlignCenter, AlignBottom, "by");


  canvas_set_font(canvas, FontPrimary);
  canvas_draw_str_aligned(canvas, 64, 40, AlignCenter, AlignCenter,
                          "Sacriphanius");








  int phase = model->animation_phase % 4;

  canvas_set_font(canvas, FontSecondary);



  int center_x = 64;
  int text_half_w = 40;

  if (phase >= 1) {
    canvas_draw_str(canvas, center_x - text_half_w - 4, 40, "(");
    canvas_draw_str(canvas, center_x + text_half_w, 40, ")");
  }
  if (phase >= 2) {
    canvas_draw_str(canvas, center_x - text_half_w - 8, 40, "(");
    canvas_draw_str(canvas, center_x + text_half_w + 4, 40, ")");
  }
  if (phase >= 3) {
    canvas_draw_str(canvas, center_x - text_half_w - 12, 40, "(");
    canvas_draw_str(canvas, center_x + text_half_w + 8, 40, ")");
  }
}

SplashView *splash_view_alloc() {
  SplashView *instance = malloc(sizeof(SplashView));
  instance->view = view_alloc();
  view_allocate_model(instance->view, ViewModelTypeLocking,
                      sizeof(SplashViewModel));
  view_set_context(instance->view, instance);
  view_set_draw_callback(instance->view, splash_view_draw_callback);

  instance->animation_phase = 0;

  return instance;
}

void splash_view_free(SplashView *instance) {
  furi_assert(instance);
  view_free(instance->view);
  free(instance);
}

View *splash_view_get_view(SplashView *instance) {
  furi_assert(instance);
  return instance->view;
}

void splash_view_timer_tick(SplashView *instance) {
  furi_assert(instance);
  instance->animation_phase++;
  with_view_model(
      instance->view, SplashViewModel * model,
      { model->animation_phase = instance->animation_phase; }, true);
}
