#pragma once

#include <stdbool.h>
#include <stdint.h>

#define FOCUS_MINUTES_MIN     15
#define FOCUS_MINUTES_MAX     60
#define FOCUS_MINUTES_DEFAULT 25

#define SHORT_BREAK_MIN     3
#define SHORT_BREAK_MAX     15
#define SHORT_BREAK_DEFAULT 5

#define LONG_BREAK_MIN     10
#define LONG_BREAK_MAX     30
#define LONG_BREAK_DEFAULT 15

#define ROUNDS_MIN     2
#define ROUNDS_MAX     6
#define ROUNDS_DEFAULT 4

#define SETTINGS_FILE_PATH    APP_DATA_PATH("settings.conf")
#define SETTINGS_FILE_TYPE    "Pomadoro Settings"
#define SETTINGS_FILE_VERSION 1

typedef enum {
    PomadoroVibroOff,
    PomadoroVibroShort,
    PomadoroVibroLong,
    PomadoroVibroCount,
} PomadoroVibroMode;

typedef enum {
    PomadoroSoundOff,
    PomadoroSoundOn,
    PomadoroSoundCount,
} PomadoroSoundMode;

typedef struct {
    uint32_t focus_minutes;
    uint32_t short_break_minutes;
    uint32_t long_break_minutes;
    uint32_t rounds;
    uint32_t vibro_mode;
    uint32_t sound_mode;
} PomadoroSettings;

void pomadoro_settings_set_defaults(PomadoroSettings* settings);
bool pomadoro_settings_load(PomadoroSettings* settings);
bool pomadoro_settings_save(const PomadoroSettings* settings);
