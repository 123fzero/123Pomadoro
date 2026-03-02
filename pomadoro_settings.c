#include "pomadoro_settings.h"

#include <flipper_format/flipper_format.h>
#include <furi.h>
#include <storage/storage.h>

void pomadoro_settings_set_defaults(PomadoroSettings* settings) {
    settings->focus_minutes = FOCUS_MINUTES_DEFAULT;
    settings->short_break_minutes = SHORT_BREAK_DEFAULT;
    settings->long_break_minutes = LONG_BREAK_DEFAULT;
    settings->rounds = ROUNDS_DEFAULT;
    settings->vibro_mode = PomadoroVibroShort;
    settings->sound_mode = PomadoroSoundOn;
}

bool pomadoro_settings_save(const PomadoroSettings* settings) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(storage);
    bool success = false;

    do {
        if(!flipper_format_file_open_always(ff, SETTINGS_FILE_PATH)) break;
        if(!flipper_format_write_header_cstr(ff, SETTINGS_FILE_TYPE, SETTINGS_FILE_VERSION)) break;
        if(!flipper_format_write_uint32(ff, "FocusMinutes", &settings->focus_minutes, 1)) break;
        if(!flipper_format_write_uint32(
               ff, "ShortBreakMinutes", &settings->short_break_minutes, 1))
            break;
        if(!flipper_format_write_uint32(ff, "LongBreakMinutes", &settings->long_break_minutes, 1))
            break;
        if(!flipper_format_write_uint32(ff, "Rounds", &settings->rounds, 1)) break;
        if(!flipper_format_write_uint32(ff, "VibroMode", &settings->vibro_mode, 1)) break;
        if(!flipper_format_write_uint32(ff, "SoundMode", &settings->sound_mode, 1)) break;
        success = true;
    } while(false);

    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
    return success;
}

bool pomadoro_settings_load(PomadoroSettings* settings) {
    pomadoro_settings_set_defaults(settings);

    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* ff = flipper_format_file_alloc(storage);
    bool success = false;

    do {
        if(!flipper_format_file_open_existing(ff, SETTINGS_FILE_PATH)) break;

        FuriString* filetype = furi_string_alloc();
        uint32_t version = 0;
        if(!flipper_format_read_header(ff, filetype, &version)) {
            furi_string_free(filetype);
            break;
        }
        if(furi_string_cmp_str(filetype, SETTINGS_FILE_TYPE) != 0 ||
           version != SETTINGS_FILE_VERSION) {
            furi_string_free(filetype);
            break;
        }
        furi_string_free(filetype);

        flipper_format_read_uint32(ff, "FocusMinutes", &settings->focus_minutes, 1);
        flipper_format_read_uint32(ff, "ShortBreakMinutes", &settings->short_break_minutes, 1);
        flipper_format_read_uint32(ff, "LongBreakMinutes", &settings->long_break_minutes, 1);
        flipper_format_read_uint32(ff, "Rounds", &settings->rounds, 1);
        flipper_format_read_uint32(ff, "VibroMode", &settings->vibro_mode, 1);
        flipper_format_read_uint32(ff, "SoundMode", &settings->sound_mode, 1);

        if(settings->focus_minutes < FOCUS_MINUTES_MIN ||
           settings->focus_minutes > FOCUS_MINUTES_MAX) {
            settings->focus_minutes = FOCUS_MINUTES_DEFAULT;
        }
        if(settings->short_break_minutes < SHORT_BREAK_MIN ||
           settings->short_break_minutes > SHORT_BREAK_MAX) {
            settings->short_break_minutes = SHORT_BREAK_DEFAULT;
        }
        if(settings->long_break_minutes < LONG_BREAK_MIN ||
           settings->long_break_minutes > LONG_BREAK_MAX) {
            settings->long_break_minutes = LONG_BREAK_DEFAULT;
        }
        if(settings->rounds < ROUNDS_MIN || settings->rounds > ROUNDS_MAX) {
            settings->rounds = ROUNDS_DEFAULT;
        }
        if(settings->vibro_mode >= PomadoroVibroCount) {
            settings->vibro_mode = PomadoroVibroShort;
        }
        if(settings->sound_mode >= PomadoroSoundCount) {
            settings->sound_mode = PomadoroSoundOn;
        }

        success = true;
    } while(false);

    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);
    return success;
}
