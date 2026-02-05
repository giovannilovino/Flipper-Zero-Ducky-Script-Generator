#include "ducky_gen_app_i.h"
#include <furi.h>
#include <gui/gui.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

static bool ducky_gen_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    DuckyGenApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool ducky_gen_back_event_callback(void* context) {
    furi_assert(context);
    DuckyGenApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void ducky_gen_tick_event_callback(void* context) {
    furi_assert(context);
    DuckyGenApp* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

DuckyGenApp* ducky_gen_app_alloc() {
    DuckyGenApp* app = malloc(sizeof(DuckyGenApp));

    app->gui = furi_record_open(RECORD_GUI);
    app->storage = furi_record_open(RECORD_STORAGE);
    app->notification = furi_record_open(RECORD_NOTIFICATION);
    app->dialogs = furi_record_open(RECORD_DIALOGS);

    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&ducky_gen_scene_handlers, app);

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, ducky_gen_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, ducky_gen_back_event_callback);
    view_dispatcher_set_tick_event_callback(
        app->view_dispatcher, ducky_gen_tick_event_callback, 100);

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);


    app->os_selection_view = os_selection_view_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        DuckyGenViewOsSelection,
        os_selection_view_get_view(app->os_selection_view));

    app->editor_view = editor_view_alloc(app->view_dispatcher);
    view_dispatcher_add_view(
        app->view_dispatcher, DuckyGenViewEditor, editor_view_get_view(app->editor_view));

    app->submenu = submenu_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DuckyGenViewSubmenu, submenu_get_view(app->submenu));

    app->text_input = text_input_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DuckyGenViewTextInput, text_input_get_view(app->text_input));

    app->splash_view = splash_view_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DuckyGenViewSplash, splash_view_get_view(app->splash_view));

    app->save_progress_view = save_progress_view_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        DuckyGenViewSaveProgress,
        save_progress_view_get_view(app->save_progress_view));

    app->widget = widget_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DuckyGenViewWidget, widget_get_view(app->widget));


    memset(app->os_name, 0, sizeof(app->os_name));
    app->file_path = furi_string_alloc();
    app->is_building_modifier = false;


    if(storage_common_stat(app->storage, "/ext/badusb/DuckyGen", NULL) != FSE_OK) {
        storage_common_mkdir(app->storage, "/ext/badusb/DuckyGen");
    }

    scene_manager_next_scene(app->scene_manager, DuckyGenSceneSplash);

    return app;
}

void ducky_gen_app_free(DuckyGenApp* app) {
    furi_assert(app);

    view_dispatcher_remove_view(app->view_dispatcher, DuckyGenViewOsSelection);
    view_dispatcher_remove_view(app->view_dispatcher, DuckyGenViewEditor);
    view_dispatcher_remove_view(app->view_dispatcher, DuckyGenViewSubmenu);
    view_dispatcher_remove_view(app->view_dispatcher, DuckyGenViewTextInput);
    view_dispatcher_remove_view(app->view_dispatcher, DuckyGenViewSplash);
    view_dispatcher_remove_view(app->view_dispatcher, DuckyGenViewSaveProgress);
    view_dispatcher_remove_view(app->view_dispatcher, DuckyGenViewWidget);

    os_selection_view_free(app->os_selection_view);
    editor_view_free(app->editor_view);
    submenu_free(app->submenu);
    text_input_free(app->text_input);
    splash_view_free(app->splash_view);
    save_progress_view_free(app->save_progress_view);
    widget_free(app->widget);

    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    furi_string_free(app->file_path);

    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_DIALOGS);

    free(app);
}

int32_t ducky_gen_app(void* p) {
    UNUSED(p);
    DuckyGenApp* app = ducky_gen_app_alloc();
    view_dispatcher_run(app->view_dispatcher);
    ducky_gen_app_free(app);
    return 0;
}
