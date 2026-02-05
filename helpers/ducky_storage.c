#include <furi.h>
#include <storage/storage.h>

#define DUCKY_GEN_PATH "/ext/badusb/DuckyGen"

bool ducky_gen_check_and_create_dir(Storage *storage, const char *os_name) {
  if (!storage_common_stat(storage, DUCKY_GEN_PATH, NULL)) {
    if (storage_common_mkdir(storage, DUCKY_GEN_PATH) != FSE_OK) {
      return false;
    }
  }


  FuriString *path = furi_string_alloc_printf("%s/%s", DUCKY_GEN_PATH, os_name);
  bool result = false;
  if (storage_common_stat(storage, furi_string_get_cstr(path), NULL) ==
      FSE_OK) {
    result = true;
  } else {
    if (storage_common_mkdir(storage, furi_string_get_cstr(path)) == FSE_OK) {
      result = true;
    }
  }

  furi_string_free(path);
  return result;
}
