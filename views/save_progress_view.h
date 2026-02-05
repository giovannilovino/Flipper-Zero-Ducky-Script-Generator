#pragma once

#include <gui/view.h>

typedef struct SaveProgressView SaveProgressView;
typedef void (*SaveProgressViewCallback)(void* context, int8_t index);

SaveProgressView* save_progress_view_alloc();
void save_progress_view_free(SaveProgressView* instance);
View* save_progress_view_get_view(SaveProgressView* instance);

void save_progress_view_set_progress(SaveProgressView* instance, float progress);
void save_progress_view_set_success(SaveProgressView* instance, bool success);

void save_progress_view_set_callback(
    SaveProgressView* instance,
    SaveProgressViewCallback callback,
    void* context);
