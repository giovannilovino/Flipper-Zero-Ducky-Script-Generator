#include "../ducky_gen_app_i.h"
#include <dialogs/dialogs.h>

enum {
    MenuIndexNewWindows = 0,
    MenuIndexNewLinux,
    MenuIndexNewMacOS,
    MenuIndexNewAndroid,
    MenuIndexLoad,
    MenuIndexAbout,
    MenuIndexQuit,
};

void ducky_gen_scene_main_menu_callback(void* context, uint32_t index) {
    DuckyGenApp* app = context;


    if(index == MenuIndexNewWindows || index == MenuIndexNewLinux || index == MenuIndexNewMacOS ||
       index == MenuIndexNewAndroid) {



        const char* os_name = "Unknown";
        if(index == MenuIndexNewWindows)
            os_name = "Windows";
        else if(index == MenuIndexNewLinux)
            os_name = "Linux";
        else if(index == MenuIndexNewMacOS)
            os_name = "macOS";
        else if(index == MenuIndexNewAndroid)
            os_name = "Android";

        editor_view_set_os_name(app->editor_view, os_name);

        strncpy(app->os_name, os_name, sizeof(app->os_name) - 1);
        app->os_name[sizeof(app->os_name) - 1] = '\0';


        editor_view_set_script(app->editor_view, ducky_script_alloc());


        scene_manager_next_scene(app->scene_manager, DuckyGenSceneEditor);

    } else if(index == MenuIndexLoad) {
        DialogsFileBrowserOptions browser_options;
        dialog_file_browser_set_basic_options(
            &browser_options,
            ".txt",
            NULL);
        browser_options.base_path = "/ext/badusb/DuckyGen";

        bool success = dialog_file_browser_show(
            app->dialogs, app->file_path, app->file_path, &browser_options);

        if(success) {

            DuckyScript* script = ducky_script_alloc();
            if(ducky_script_load_from_file(
                   script, app->storage, furi_string_get_cstr(app->file_path))) {
                editor_view_set_script(app->editor_view, script);


                editor_view_set_os_name(app->editor_view, "Loaded");
                strncpy(app->os_name, "Loaded", sizeof(app->os_name) - 1);
                app->os_name[sizeof(app->os_name) - 1] = '\0';

                scene_manager_next_scene(app->scene_manager, DuckyGenSceneEditor);
            } else {
                ducky_script_free(script);

            }
        }
    } else if(index == MenuIndexAbout) {
        DialogMessage* message = dialog_message_alloc();
        dialog_message_set_header(message, "About DuckyGen", 64, 0, AlignLeft, AlignTop);
        dialog_message_set_text(
            message,
            "Educational Purposes Only!\nUse responsibly.",
            64,
            32,
            AlignCenter,
            AlignCenter);
        dialog_message_set_buttons(message, "OK", NULL, NULL);
        dialog_message_show(app->dialogs, message);
        dialog_message_free(message);

    } else if(index == MenuIndexQuit) {
        view_dispatcher_stop(app->view_dispatcher);
    }
}

void ducky_gen_scene_main_menu_on_enter(void* context) {
    DuckyGenApp* app = context;


    os_selection_view_set_callback(
        app->os_selection_view, ducky_gen_scene_main_menu_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewOsSelection);
}

bool ducky_gen_scene_main_menu_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void ducky_gen_scene_main_menu_on_exit(void* context) {
    UNUSED(context);
}
