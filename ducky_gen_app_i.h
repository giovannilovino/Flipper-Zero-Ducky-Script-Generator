#pragma once

#include <dialogs/dialogs.h>
#include <furi.h>
#include <gui/gui.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>
#include <notification/notification_app.h>
#include <storage/storage.h>
#include <loader/loader.h>

#include "helpers/ducky_script.h"
#include "scenes/ducky_gen_scene.h"
#include "views/editor_view.h"
#include "views/os_selection_view.h"
#include "views/save_progress_view.h"
#include "views/splash_view.h"

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    NotificationApp* notification;
    DialogsApp* dialogs;
    Storage* storage;


    OsSelectionView* os_selection_view;
    EditorView* editor_view;
    SplashView* splash_view;
    Submenu* submenu;
    TextInput* text_input;
    SaveProgressView* save_progress_view;
    Widget* widget;


    char os_name[32];
    char text_input_buffer[128];
    FuriString* file_path;
    ScriptLineType pending_line_type;


    char modifier_str[64];
    bool is_building_modifier;


    uint32_t focused_line_index;
    bool is_editing;

} DuckyGenApp;

typedef enum {
    DuckyGenViewOsSelection,
    DuckyGenViewEditor,
    DuckyGenViewSplash,
    DuckyGenViewSubmenu,
    DuckyGenViewTextInput,
    DuckyGenViewSaveProgress,
    DuckyGenViewWidget,
} DuckyGenView;
