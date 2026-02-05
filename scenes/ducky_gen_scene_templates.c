#include "../ducky_gen_app_i.h"

typedef struct {
  const char *name;
  const char *payload;
} DuckyTemplate;


enum {
  TemplateWinPowershellAdmin,
  TemplateWinNotepad,
  TemplateWinDisableDefender,
  TemplateLinuxTerminal,
  TemplateHelloWorld,
};

void ducky_gen_scene_templates_submenu_callback(void *context, uint32_t index) {
  DuckyGenApp *app = context;
  view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void ducky_gen_scene_templates_on_enter(void *context) {
  DuckyGenApp *app = context;
  Submenu *submenu = app->submenu;

  submenu_reset(submenu);
  submenu_set_header(submenu, "Ready Templates");

  submenu_add_item(submenu, "[Win] Powershell Admin",
                   TemplateWinPowershellAdmin,
                   ducky_gen_scene_templates_submenu_callback, app);
  submenu_add_item(submenu, "[Win] Notepad Hello", TemplateWinNotepad,
                   ducky_gen_scene_templates_submenu_callback, app);
  submenu_add_item(submenu, "[Win] Disable Defender",
                   TemplateWinDisableDefender,
                   ducky_gen_scene_templates_submenu_callback, app);
  submenu_add_item(submenu, "[Linux] Open Terminal", TemplateLinuxTerminal,
                   ducky_gen_scene_templates_submenu_callback, app);
  submenu_add_item(submenu, "Hello World (String)", TemplateHelloWorld,
                   ducky_gen_scene_templates_submenu_callback, app);

  view_dispatcher_switch_to_view(app->view_dispatcher, DuckyGenViewSubmenu);
}

bool ducky_gen_scene_templates_on_event(void *context,
                                        SceneManagerEvent event) {
  DuckyGenApp *app = context;
  DuckyScript *script = editor_view_get_script(app->editor_view);
  uint32_t pos = app->focused_line_index;

  if (event.type == SceneManagerEventTypeCustom) {
    if (event.event == TemplateWinPowershellAdmin) {
      ducky_script_insert_line(script, pos++, ScriptLineTypeCombo, "GUI r");
      ducky_script_insert_line(script, pos++, ScriptLineTypeDelay, "500");
      ducky_script_insert_line(
          script, pos++, ScriptLineTypeString,
          "powershell Start-Process powershell -Verb runAs");
      ducky_script_insert_line(script, pos++, ScriptLineTypeEnter, NULL);
    } else if (event.event == TemplateWinNotepad) {
      ducky_script_insert_line(script, pos++, ScriptLineTypeCombo, "GUI r");
      ducky_script_insert_line(script, pos++, ScriptLineTypeDelay, "500");
      ducky_script_insert_line(script, pos++, ScriptLineTypeString, "notepad");
      ducky_script_insert_line(script, pos++, ScriptLineTypeEnter, NULL);
      ducky_script_insert_line(script, pos++, ScriptLineTypeDelay, "500");
      ducky_script_insert_line(script, pos++, ScriptLineTypeString,
                               "Hello from Flipper Zero!");
    } else if (event.event == TemplateWinDisableDefender) {
      ducky_script_insert_line(script, pos++, ScriptLineTypeCombo, "GUI r");
      ducky_script_insert_line(script, pos++, ScriptLineTypeDelay, "500");
      ducky_script_insert_line(script, pos++, ScriptLineTypeString,
                               "powershell");
      ducky_script_insert_line(script, pos++, ScriptLineTypeEnter, NULL);
      ducky_script_insert_line(script, pos++, ScriptLineTypeDelay, "1000");
      ducky_script_insert_line(
          script, pos++, ScriptLineTypeString,
          "Set-MpPreference -DisableRealtimeMonitoring $true");
      ducky_script_insert_line(script, pos++, ScriptLineTypeEnter, NULL);
    } else if (event.event == TemplateLinuxTerminal) {
      ducky_script_insert_line(script, pos++, ScriptLineTypeCombo,
                               "CTRL ALT t");
    } else if (event.event == TemplateHelloWorld) {
      ducky_script_insert_line(script, pos++, ScriptLineTypeString,
                               "Hello World!");
    }

    scene_manager_search_and_switch_to_previous_scene(app->scene_manager,
                                                      DuckyGenSceneEditor);
    return true;
  }
  return false;
}

void ducky_gen_scene_templates_on_exit(void *context) {
  DuckyGenApp *app = context;
  submenu_reset(app->submenu);
}
