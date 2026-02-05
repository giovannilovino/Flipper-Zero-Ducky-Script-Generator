#include "../ducky_gen_app_i.h"


enum {
    SubmenuIndexInputDelay,
    SubmenuIndexActionKeys,
    SubmenuIndexNavigation,
    SubmenuIndexFunctionKeys,
    SubmenuIndexModifiers,
    SubmenuIndexTemplates,
    SubmenuIndexAdvanced,
};


void ducky_gen_scene_command_type_submenu_callback(void* context, uint32_t index) {
    DuckyGenApp* app = context;
    if(index == SubmenuIndexInputDelay) {
        scene_manager_set_scene_state(
            app->scene_manager, DuckyGenSceneCommandType, SubmenuIndexInputDelay);
        scene_manager_next_scene(app->scene_manager, DuckyGenSceneCommandCategory);
    } else if(index == SubmenuIndexModifiers) {

        app->is_building_modifier = true;
        app->modifier_str[0] = '\0';
        scene_manager_next_scene(app->scene_manager,
                                 DuckyGenSceneModifierBuilder);
    } else if(index == SubmenuIndexTemplates) {
        scene_manager_next_scene(app->scene_manager, DuckyGenSceneTemplates);
    } else {







        scene_manager_set_scene_state(app->scene_manager, DuckyGenSceneCommandType, index);
        scene_manager_next_scene(
            app->scene_manager,
            DuckyGenSceneCommandCategory);
    }
}

void ducky_gen_scene_command_type_on_enter(void* context) {
    DuckyGenApp* app = context;
    submenu_reset(app->submenu);

    submenu_add_item(
        app->submenu,
        "Input & Delay",
        SubmenuIndexInputDelay,
        ducky_gen_scene_command_type_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Action Keys (Enter/Tab..)",
        SubmenuIndexActionKeys,
        ducky_gen_scene_command_type_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Navigation (Arrows..)",
        SubmenuIndexNavigation,
        ducky_gen_scene_command_type_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Function Keys (F1-F12)",
        SubmenuIndexFunctionKeys,
        ducky_gen_scene_command_type_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Modifiers (CTRL/ALT..)",
        SubmenuIndexModifiers,
        ducky_gen_scene_command_type_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Library (Templates)",
        SubmenuIndexTemplates,
        ducky_gen_scene_command_type_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Advanced (Hold/Repeat..)",
        SubmenuIndexAdvanced,
        ducky_gen_scene_command_type_submenu_callback,
        app);

    submenu_set_header(app->submenu, "Select Category");
    view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewSubmenu);
}

bool ducky_gen_scene_command_type_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void ducky_gen_scene_command_type_on_exit(void* context) {
    UNUSED(context);
}
