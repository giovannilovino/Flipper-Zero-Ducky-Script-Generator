#pragma once

#include <gui/modules/widget.h>
#include <gui/view.h>

typedef void (*OsSelectionViewCallback)(void *context, uint32_t index);

typedef struct OsSelectionView OsSelectionView;

OsSelectionView *os_selection_view_alloc();

void os_selection_view_free(OsSelectionView *view);

View *os_selection_view_get_view(OsSelectionView *view);

void os_selection_view_set_callback(OsSelectionView *view,
                                    OsSelectionViewCallback callback,
                                    void *context);
