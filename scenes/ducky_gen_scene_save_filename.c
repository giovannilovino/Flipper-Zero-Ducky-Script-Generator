#include "../ducky_gen_app_i.h"
#include <dialogs/dialogs.h>

void ducky_gen_scene_save_filename_callback(void* context) {
    DuckyGenApp* app = context;


    char filename[128];
    strlcpy(filename, app->text_input_buffer, sizeof(filename));


    if(!strstr(filename, ".txt")) {
        strlcat(filename, ".txt", sizeof(filename));
    }


    if(storage_common_stat(app->storage, "/ext/badusb/DuckyGen", NULL) != FSE_OK) {
        storage_common_mkdir(app->storage, "/ext/badusb/DuckyGen");
    }


    FuriString* save_dir = furi_string_alloc();
    furi_string_printf(save_dir, "/ext/badusb/DuckyGen/%s", app->os_name);
    if(storage_common_stat(app->storage, furi_string_get_cstr(save_dir), NULL) != FSE_OK) {
        storage_common_mkdir(app->storage, furi_string_get_cstr(save_dir));
    }


    furi_string_printf(app->file_path, "%s/%s", furi_string_get_cstr(save_dir), filename);

    furi_string_free(save_dir);


    scene_manager_next_scene(app->scene_manager, DuckyGenSceneSaveProgress);
}

void ducky_gen_scene_save_filename_on_enter(void* context) {
    DuckyGenApp* app = context;
    text_input_reset(app->text_input);
    text_input_set_header_text(app->text_input, "Filename (.txt)");


    strlcpy(app->text_input_buffer, "payload", sizeof(app->text_input_buffer));

    text_input_set_result_callback(
        app->text_input,
        ducky_gen_scene_save_filename_callback,
        app,
        app->text_input_buffer,
        sizeof(app->text_input_buffer),
        true);

    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewTextInput);
}

bool ducky_gen_scene_save_filename_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void ducky_gen_scene_save_filename_on_exit(void* context) {
    UNUSED(context);
}
