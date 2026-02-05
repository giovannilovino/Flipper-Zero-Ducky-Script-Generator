#include "ducky_script.h"

DuckyScript* ducky_script_alloc() {
    DuckyScript* script = malloc(sizeof(DuckyScript));
    script->head = NULL;
    script->tail = NULL;
    script->count = 0;
    return script;
}

void ducky_script_free(DuckyScript* script) {
    if(!script) return;
    ScriptLine* current = script->head;
    while(current) {
        ScriptLine* next = current->next;
        if(current->parameter) free(current->parameter);
        free(current);
        current = next;
    }
    free(script);
}

void ducky_script_add_line(DuckyScript* script, ScriptLineType type, const char* parameter) {
    if(!script) return;

    ScriptLine* new_line = malloc(sizeof(ScriptLine));
    new_line->type = type;
    new_line->next = NULL;

    if(parameter) {
        new_line->parameter = strdup(parameter);
    } else {
        new_line->parameter = NULL;
    }

    if(script->tail) {
        script->tail->next = new_line;
        script->tail = new_line;
    } else {
        script->head = new_line;
        script->tail = new_line;
    }
    script->count++;
}

ScriptLine* ducky_script_get_line(DuckyScript* script, uint32_t index) {
    if(!script || index >= script->count) return NULL;

    ScriptLine* current = script->head;
    for(uint32_t i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

uint32_t ducky_script_get_count(DuckyScript* script) {
    return script ? script->count : 0;
}

const char* ducky_script_get_type_str(ScriptLineType type) {
    switch(type) {
    case ScriptLineTypeString:
        return "STRING";
    case ScriptLineTypeStringLn:
        return "STRINGLN";
    case ScriptLineTypeRem:
        return "REM";
    case ScriptLineTypeDelay:
        return "DELAY";
    case ScriptLineTypeDefaultDelay:
        return "DEFAULT_DELAY";
    case ScriptLineTypeAlt:
        return "ALT";
    case ScriptLineTypeAltGr:
        return "ALTGR";
    case ScriptLineTypeCtrl:
        return "CTRL";
    case ScriptLineTypeGui:
        return "GUI";
    case ScriptLineTypeShift:
        return "SHIFT";
    case ScriptLineTypeEnter:
        return "ENTER";
    case ScriptLineTypeTab:
        return "TAB";
    case ScriptLineTypeSpace:
        return "SPACE";
    case ScriptLineTypeEsc:
        return "ESC";
    case ScriptLineTypeBackspace:
        return "BACKSPACE";
    case ScriptLineTypeDelete:
        return "DELETE";
    case ScriptLineTypeInsert:
        return "INSERT";
    case ScriptLineTypeHome:
        return "HOME";
    case ScriptLineTypeEnd:
        return "END";
    case ScriptLineTypePageUp:
        return "PAGEUP";
    case ScriptLineTypePageDown:
        return "PAGEDOWN";
    case ScriptLineTypeCapsLock:
        return "CAPSLOCK";
    case ScriptLineTypeNumLock:
        return "NUMLOCK";
    case ScriptLineTypeScrollLock:
        return "SCROLLLOCK";
    case ScriptLineTypePrintScreen:
        return "PRINTSCREEN";
    case ScriptLineTypePause:
        return "PAUSE";
    case ScriptLineTypeBreak:
        return "BREAK";
    case ScriptLineTypeMenu:
        return "MENU";
    case ScriptLineTypeApp:
        return "APP";
    case ScriptLineTypeF1:
        return "F1";
    case ScriptLineTypeF2:
        return "F2";
    case ScriptLineTypeF3:
        return "F3";
    case ScriptLineTypeF4:
        return "F4";
    case ScriptLineTypeF5:
        return "F5";
    case ScriptLineTypeF6:
        return "F6";
    case ScriptLineTypeF7:
        return "F7";
    case ScriptLineTypeF8:
        return "F8";
    case ScriptLineTypeF9:
        return "F9";
    case ScriptLineTypeF10:
        return "F10";
    case ScriptLineTypeF11:
        return "F11";
    case ScriptLineTypeF12:
        return "F12";
    case ScriptLineTypeUp:
        return "UP";
    case ScriptLineTypeDown:
        return "DOWN";
    case ScriptLineTypeLeft:
        return "LEFT";
    case ScriptLineTypeRight:
        return "RIGHT";
    case ScriptLineTypeCombo:
        return "";
    case ScriptLineTypeRepeat:
        return "REPEAT";
    case ScriptLineTypeHold:
        return "HOLD";
    case ScriptLineTypeRelease:
        return "RELEASE";
    default:
        return "CMD";
    }
}

bool ducky_script_save(DuckyScript* script, Storage* storage, const char* path) {
    if(!script || !storage || !path) return false;

    File* file = storage_file_alloc(storage);
    if(!storage_file_open(file, path, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        storage_file_free(file);
        return false;
    }

    ScriptLine* current = script->head;
    bool success = true;
    while(current) {
        FuriString* line_str = furi_string_alloc();

        if(current->type == ScriptLineTypeCombo) {
            if(current->parameter) furi_string_cat(line_str, current->parameter);
        } else {
            furi_string_cat_printf(line_str, "%s", ducky_script_get_type_str(current->type));
            if(current->parameter) {
                furi_string_cat_printf(line_str, " %s", current->parameter);
            }
        }
        furi_string_cat_printf(line_str, "\n");

        if(storage_file_write(file, furi_string_get_cstr(line_str), furi_string_size(line_str)) !=
           furi_string_size(line_str)) {
            success = false;
        }

        furi_string_free(line_str);
        if(!success) break;
        current = current->next;
    }

    storage_file_close(file);
    storage_file_free(file);
    return success;
}


static ScriptLineType ducky_script_get_type_from_str(const char* str) {
    if(strcmp(str, "STRING") == 0) return ScriptLineTypeString;
    if(strcmp(str, "STRINGLN") == 0) return ScriptLineTypeStringLn;
    if(strcmp(str, "REM") == 0) return ScriptLineTypeRem;
    if(strcmp(str, "DELAY") == 0) return ScriptLineTypeDelay;
    if(strcmp(str, "DEFAULT_DELAY") == 0) return ScriptLineTypeDefaultDelay;
    if(strcmp(str, "DEFAULTDELAY") == 0) return ScriptLineTypeDefaultDelay;

    if(strcmp(str, "ALT") == 0) return ScriptLineTypeAlt;
    if(strcmp(str, "ALTGR") == 0) return ScriptLineTypeAltGr;
    if(strcmp(str, "CTRL") == 0) return ScriptLineTypeCtrl;
    if(strcmp(str, "CONTROL") == 0) return ScriptLineTypeCtrl;
    if(strcmp(str, "GUI") == 0) return ScriptLineTypeGui;
    if(strcmp(str, "WINDOWS") == 0) return ScriptLineTypeGui;
    if(strcmp(str, "COMMAND") == 0) return ScriptLineTypeGui;
    if(strcmp(str, "SHIFT") == 0) return ScriptLineTypeShift;

    if(strcmp(str, "ENTER") == 0) return ScriptLineTypeEnter;
    if(strcmp(str, "TAB") == 0) return ScriptLineTypeTab;
    if(strcmp(str, "SPACE") == 0) return ScriptLineTypeSpace;
    if(strcmp(str, "ESC") == 0) return ScriptLineTypeEsc;
    if(strcmp(str, "ESCAPE") == 0) return ScriptLineTypeEsc;
    if(strcmp(str, "BACKSPACE") == 0) return ScriptLineTypeBackspace;
    if(strcmp(str, "DELETE") == 0) return ScriptLineTypeDelete;
    if(strcmp(str, "INSERT") == 0) return ScriptLineTypeInsert;
    if(strcmp(str, "HOME") == 0) return ScriptLineTypeHome;
    if(strcmp(str, "END") == 0) return ScriptLineTypeEnd;
    if(strcmp(str, "PAGEUP") == 0) return ScriptLineTypePageUp;
    if(strcmp(str, "PAGEDOWN") == 0) return ScriptLineTypePageDown;
    if(strcmp(str, "CAPSLOCK") == 0) return ScriptLineTypeCapsLock;
    if(strcmp(str, "NUMLOCK") == 0) return ScriptLineTypeNumLock;
    if(strcmp(str, "SCROLLLOCK") == 0) return ScriptLineTypeScrollLock;
    if(strcmp(str, "PRINTSCREEN") == 0) return ScriptLineTypePrintScreen;
    if(strcmp(str, "PAUSE") == 0) return ScriptLineTypePause;
    if(strcmp(str, "BREAK") == 0) return ScriptLineTypeBreak;
    if(strcmp(str, "MENU") == 0) return ScriptLineTypeMenu;
    if(strcmp(str, "APP") == 0) return ScriptLineTypeApp;

    if(strcmp(str, "F1") == 0) return ScriptLineTypeF1;
    if(strcmp(str, "F2") == 0) return ScriptLineTypeF2;
    if(strcmp(str, "F3") == 0) return ScriptLineTypeF3;
    if(strcmp(str, "F4") == 0) return ScriptLineTypeF4;
    if(strcmp(str, "F5") == 0) return ScriptLineTypeF5;
    if(strcmp(str, "F6") == 0) return ScriptLineTypeF6;
    if(strcmp(str, "F7") == 0) return ScriptLineTypeF7;
    if(strcmp(str, "F8") == 0) return ScriptLineTypeF8;
    if(strcmp(str, "F9") == 0) return ScriptLineTypeF9;
    if(strcmp(str, "F10") == 0) return ScriptLineTypeF10;
    if(strcmp(str, "F11") == 0) return ScriptLineTypeF11;
    if(strcmp(str, "F12") == 0) return ScriptLineTypeF12;

    if(strcmp(str, "UP") == 0) return ScriptLineTypeUp;
    if(strcmp(str, "UPARROW") == 0) return ScriptLineTypeUp;
    if(strcmp(str, "DOWN") == 0) return ScriptLineTypeDown;
    if(strcmp(str, "DOWNARROW") == 0) return ScriptLineTypeDown;
    if(strcmp(str, "LEFT") == 0) return ScriptLineTypeLeft;
    if(strcmp(str, "LEFTARROW") == 0) return ScriptLineTypeLeft;
    if(strcmp(str, "RIGHT") == 0) return ScriptLineTypeRight;
    if(strcmp(str, "RIGHTARROW") == 0) return ScriptLineTypeRight;

    if(strcmp(str, "REPEAT") == 0) return ScriptLineTypeRepeat;
    if(strcmp(str, "HOLD") == 0) return ScriptLineTypeHold;
    if(strcmp(str, "RELEASE") == 0) return ScriptLineTypeRelease;












    return ScriptLineTypeCombo;

}


static bool is_modifier(ScriptLineType type) {
    return (
        type == ScriptLineTypeCtrl || type == ScriptLineTypeAlt || type == ScriptLineTypeShift ||
        type == ScriptLineTypeGui || type == ScriptLineTypeAltGr);
}

bool ducky_script_load_from_file(DuckyScript* script, Storage* storage, const char* path) {
    File* file = storage_file_alloc(storage);
    if(!storage_file_open(file, path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        storage_file_free(file);
        return false;
    }


    while(script->head) {
        ScriptLine* next = script->head->next;
        if(script->head->parameter) free(script->head->parameter);
        free(script->head);
        script->head = next;
    }
    script->tail = NULL;
    script->count = 0;


    FuriString* line = furi_string_alloc();
    char buffer[1];
    while(storage_file_read(file, buffer, 1) > 0) {
        if(buffer[0] == '\n') {

            const char* cstr = furi_string_get_cstr(line);
            if(strlen(cstr) > 0 && cstr[strlen(cstr) - 1] == '\r') {
                furi_string_trim(line);
            }


            char* str_copy = strdup(furi_string_get_cstr(line));
            char* first_space = strchr(str_copy, ' ');

            char* cmd = str_copy;
            char* param = NULL;

            if(first_space) {
                *first_space = '\0';
                param = first_space + 1;
            }

            ScriptLineType type = ducky_script_get_type_from_str(cmd);







            if(type == ScriptLineTypeCombo) {


                ducky_script_add_line(script, ScriptLineTypeCombo, furi_string_get_cstr(line));
            } else if(is_modifier(type) && param != NULL) {











                ducky_script_add_line(script, ScriptLineTypeCombo, furi_string_get_cstr(line));
            } else {

                ducky_script_add_line(script, type, param);
            }

            free(str_copy);
            furi_string_reset(line);
        } else {
            furi_string_push_back(line, buffer[0]);
        }
    }


    if(furi_string_size(line) > 0) {





        char* str_copy = strdup(furi_string_get_cstr(line));
        char* first_space = strchr(str_copy, ' ');
        char* cmd = str_copy;
        char* param = NULL;
        if(first_space) {
            *first_space = '\0';
            param = first_space + 1;
        }
        ScriptLineType type = ducky_script_get_type_from_str(cmd);
        if(type == ScriptLineTypeCombo) {
            ducky_script_add_line(script, ScriptLineTypeCombo, furi_string_get_cstr(line));
        } else if(is_modifier(type) && param != NULL) {
            ducky_script_add_line(script, ScriptLineTypeCombo, furi_string_get_cstr(line));
        } else {
            ducky_script_add_line(script, type, param);
        }
        free(str_copy);
    }

    furi_string_free(line);
    storage_file_close(file);
    storage_file_free(file);
    return true;
}

void ducky_script_delete_line(DuckyScript* script, uint32_t index) {
    if(!script || index >= script->count) return;

    ScriptLine* prev = NULL;
    ScriptLine* current = script->head;

    for(uint32_t i = 0; i < index; i++) {
        prev = current;
        current = current->next;
    }

    if(prev) {
        prev->next = current->next;
    } else {
        script->head = current->next;
    }

    if(current == script->tail) {
        script->tail = prev;
    }

    if(current->parameter) free(current->parameter);
    free(current);
    script->count--;
}

void ducky_script_move_line_up(DuckyScript* script, uint32_t index) {
    if(!script || index == 0 || index >= script->count) return;


    ScriptLine* prev_prev = NULL;
    ScriptLine* prev = NULL;
    ScriptLine* current = script->head;

    for(uint32_t i = 0; i < index; i++) {
        prev_prev = prev;
        prev = current;
        current = current->next;
    }


    prev->next = current->next;
    current->next = prev;

    if(prev_prev) {
        prev_prev->next = current;
    } else {
        script->head = current;
    }

    if(current == script->tail) {
        script->tail = prev;
    }
}

void ducky_script_move_line_down(DuckyScript* script, uint32_t index) {
    if(!script || index >= script->count - 1) return;

    ducky_script_move_line_up(script, index + 1);
}

void ducky_script_insert_line(
    DuckyScript* script,
    uint32_t index,
    ScriptLineType type,
    const char* parameter) {
    if(!script) return;
    if(index >= script->count) {
        ducky_script_add_line(script, type, parameter);
        return;
    }

    ScriptLine* new_line = malloc(sizeof(ScriptLine));
    new_line->type = type;
    if(parameter) {
        new_line->parameter = strdup(parameter);
    } else {
        new_line->parameter = NULL;
    }

    if(index == 0) {
        new_line->next = script->head;
        script->head = new_line;
    } else {
        ScriptLine* prev = ducky_script_get_line(script, index - 1);
        new_line->next = prev->next;
        prev->next = new_line;
    }
    script->count++;
}
