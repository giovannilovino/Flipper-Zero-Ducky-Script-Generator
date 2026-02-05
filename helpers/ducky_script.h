#pragma once
#include <furi.h>
#include <storage/storage.h>

typedef enum {

  ScriptLineTypeString,
  ScriptLineTypeStringLn,
  ScriptLineTypeRem,


  ScriptLineTypeDelay,
  ScriptLineTypeDefaultDelay,


  ScriptLineTypeAlt,
  ScriptLineTypeAltGr,
  ScriptLineTypeCtrl,
  ScriptLineTypeGui,
  ScriptLineTypeShift,


  ScriptLineTypeEnter,
  ScriptLineTypeTab,
  ScriptLineTypeSpace,
  ScriptLineTypeEsc,
  ScriptLineTypeBackspace,
  ScriptLineTypeDelete,
  ScriptLineTypeInsert,
  ScriptLineTypeHome,
  ScriptLineTypeEnd,
  ScriptLineTypePageUp,
  ScriptLineTypePageDown,
  ScriptLineTypeCapsLock,
  ScriptLineTypeNumLock,
  ScriptLineTypeScrollLock,
  ScriptLineTypePrintScreen,
  ScriptLineTypePause,
  ScriptLineTypeBreak,
  ScriptLineTypeMenu,
  ScriptLineTypeApp,


  ScriptLineTypeF1,
  ScriptLineTypeF2,
  ScriptLineTypeF3,
  ScriptLineTypeF4,
  ScriptLineTypeF5,
  ScriptLineTypeF6,
  ScriptLineTypeF7,
  ScriptLineTypeF8,
  ScriptLineTypeF9,
  ScriptLineTypeF10,
  ScriptLineTypeF11,
  ScriptLineTypeF12,


  ScriptLineTypeUp,
  ScriptLineTypeDown,
  ScriptLineTypeLeft,
  ScriptLineTypeRight,


  ScriptLineTypeCombo,


  ScriptLineTypeRepeat,
  ScriptLineTypeHold,
  ScriptLineTypeRelease,
} ScriptLineType;

typedef struct ScriptLine {
  ScriptLineType type;
  char *parameter;
  struct ScriptLine *next;
} ScriptLine;

typedef struct {
  ScriptLine *head;
  ScriptLine *tail;
  uint32_t count;
} DuckyScript;

DuckyScript *ducky_script_alloc();
void ducky_script_free(DuckyScript *script);
void ducky_script_add_line(DuckyScript *script, ScriptLineType type,
                           const char *parameter);
ScriptLine *ducky_script_get_line(DuckyScript *script, uint32_t index);
uint32_t ducky_script_get_count(DuckyScript *script);
const char *ducky_script_get_type_str(ScriptLineType type);
bool ducky_script_save(DuckyScript *script, Storage *storage, const char *path);
bool ducky_script_load_from_file(DuckyScript *script, Storage *storage,
                                 const char *path);
void ducky_script_delete_line(DuckyScript *script, uint32_t index);
void ducky_script_move_line_up(DuckyScript *script, uint32_t index);
void ducky_script_move_line_down(DuckyScript *script, uint32_t index);
void ducky_script_insert_line(DuckyScript *script, uint32_t index,
                              ScriptLineType type, const char *parameter);
