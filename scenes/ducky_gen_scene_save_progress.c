#include "../ducky_gen_app_i.h"

#define SAVE_TICK_COUNT 20

void ducky_gen_scene_save_progress_callback(void* context, int8_t index) {
    DuckyGenApp* app = context;
    if(index == 0) {
        Loader* loader = furi_record_open(RECORD_LOADER);


        loader_enqueue_launch(
            loader, "Bad KB", furi_string_get_cstr(app->file_path), LoaderDeferredLaunchFlagGui);
        furi_record_close(RECORD_LOADER);


        view_dispatcher_stop(app->view_dispatcher);
    } else if(index == 1) {
        scene_manager_search_and_switch_to_previous_scene(app->scene_manager, DuckyGenSceneEditor);
    } else if(index == 2) {
        view_dispatcher_stop(app->view_dispatcher);
    }
}

void ducky_gen_scene_save_progress_on_enter(void* context) {
    DuckyGenApp* app = context;
    save_progress_view_set_progress(app->save_progress_view, 0.0f);
    save_progress_view_set_success(app->save_progress_view, false);
    save_progress_view_set_callback(
        app->save_progress_view, ducky_gen_scene_save_progress_callback, app);


    bool saved = ducky_script_save(
        editor_view_get_script(app->editor_view),
        app->storage,
        furi_string_get_cstr(app->file_path));

    if(!saved) {
        notification_message(app->notification, &sequence_error);


    }

    scene_manager_set_scene_state(app->scene_manager, DuckyGenSceneSaveProgress, 0);
    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewSaveProgress);
}

bool ducky_gen_scene_save_progress_on_event(void* context, SceneManagerEvent event) {
    DuckyGenApp* app = context;
    if(event.type == SceneManagerEventTypeTick) {
        uint32_t state =
            scene_manager_get_scene_state(app->scene_manager, DuckyGenSceneSaveProgress);

        bool success_flag = false;
        UNUSED(success_flag);

        if(state <= SAVE_TICK_COUNT) {
            state++;
            float progress = (float)state / (float)SAVE_TICK_COUNT;
            save_progress_view_set_progress(app->save_progress_view, progress);
            scene_manager_set_scene_state(app->scene_manager, DuckyGenSceneSaveProgress, state);

            if(state == SAVE_TICK_COUNT) {


                save_progress_view_set_success(app->save_progress_view, true);
                notification_message(app->notification, &sequence_success);
            }
        }
        return true;
    }
    return false;
}

void ducky_gen_scene_save_progress_on_exit(void* context) {
    UNUSED(context);
}
