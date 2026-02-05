#pragma once

#include "../helpers/ducky_script.h"
#include <gui/view.h>
#include <gui/view_dispatcher.h>

typedef struct EditorView EditorView;

EditorView *editor_view_alloc(ViewDispatcher *view_dispatcher);

void editor_view_free(EditorView *view);

View *editor_view_get_view(EditorView *view);

void editor_view_set_os_name(EditorView *view, const char *os_name);

DuckyScript *editor_view_get_script(EditorView *view);

void editor_view_set_script(EditorView *view, DuckyScript *script);

int32_t editor_view_get_selected_index(EditorView *view);
