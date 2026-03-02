#include "../pomadoro_app.h"
#include "pomadoro_scene.h"

#include <stdio.h>

static const char* const vibro_labels[] = {"Off", "Short", "Long"};
static const char* const sound_labels[] = {"Off", "On"};

enum {
    PomadoroSettingsIndexFocus,
    PomadoroSettingsIndexShortBreak,
    PomadoroSettingsIndexLongBreak,
    PomadoroSettingsIndexRounds,
    PomadoroSettingsIndexVibration,
    PomadoroSettingsIndexSound,
    PomadoroSettingsIndexFactoryReset,
};

static void pomadoro_scene_settings_build_list(PomadoroApp* app);

static void pomadoro_scene_settings_focus_changed(VariableItem* item) {
    PomadoroApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    uint32_t value = FOCUS_MINUTES_MIN + index;
    char buf[8];

    app->settings.focus_minutes = value;
    snprintf(buf, sizeof(buf), "%lum", (unsigned long)value);
    variable_item_set_current_value_text(item, buf);
    pomadoro_settings_save(&app->settings);
}

static void pomadoro_scene_settings_short_break_changed(VariableItem* item) {
    PomadoroApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    uint32_t value = SHORT_BREAK_MIN + index;
    char buf[8];

    app->settings.short_break_minutes = value;
    snprintf(buf, sizeof(buf), "%lum", (unsigned long)value);
    variable_item_set_current_value_text(item, buf);
    pomadoro_settings_save(&app->settings);
}

static void pomadoro_scene_settings_long_break_changed(VariableItem* item) {
    PomadoroApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    uint32_t value = LONG_BREAK_MIN + index;
    char buf[8];

    app->settings.long_break_minutes = value;
    snprintf(buf, sizeof(buf), "%lum", (unsigned long)value);
    variable_item_set_current_value_text(item, buf);
    pomadoro_settings_save(&app->settings);
}

static void pomadoro_scene_settings_rounds_changed(VariableItem* item) {
    PomadoroApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    uint32_t value = ROUNDS_MIN + index;
    char buf[8];

    app->settings.rounds = value;
    snprintf(buf, sizeof(buf), "%lu", (unsigned long)value);
    variable_item_set_current_value_text(item, buf);
    pomadoro_settings_save(&app->settings);
}

static void pomadoro_scene_settings_vibro_changed(VariableItem* item) {
    PomadoroApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);

    app->settings.vibro_mode = index;
    variable_item_set_current_value_text(item, vibro_labels[index]);
    pomadoro_settings_save(&app->settings);
}

static void pomadoro_scene_settings_sound_changed(VariableItem* item) {
    PomadoroApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);

    app->settings.sound_mode = index;
    variable_item_set_current_value_text(item, sound_labels[index]);
    pomadoro_settings_save(&app->settings);
}

static void pomadoro_scene_settings_enter_callback(void* context, uint32_t index) {
    PomadoroApp* app = context;

    if(index == PomadoroSettingsIndexFactoryReset) {
        pomadoro_settings_set_defaults(&app->settings);
        pomadoro_settings_save(&app->settings);
        pomadoro_scene_settings_build_list(app);
        variable_item_list_set_selected_item(
            app->variable_item_list, PomadoroSettingsIndexFactoryReset);
    }
}

static void pomadoro_scene_settings_build_list(PomadoroApp* app) {
    VariableItem* item;
    char buf[8];

    variable_item_list_reset(app->variable_item_list);
    variable_item_list_set_header(app->variable_item_list, "Settings");
    variable_item_list_set_enter_callback(
        app->variable_item_list, pomadoro_scene_settings_enter_callback, app);

    item = variable_item_list_add(
        app->variable_item_list,
        "Focus",
        FOCUS_MINUTES_MAX - FOCUS_MINUTES_MIN + 1,
        pomadoro_scene_settings_focus_changed,
        app);
    variable_item_set_current_value_index(item, app->settings.focus_minutes - FOCUS_MINUTES_MIN);
    snprintf(buf, sizeof(buf), "%lum", (unsigned long)app->settings.focus_minutes);
    variable_item_set_current_value_text(item, buf);

    item = variable_item_list_add(
        app->variable_item_list,
        "Short Break",
        SHORT_BREAK_MAX - SHORT_BREAK_MIN + 1,
        pomadoro_scene_settings_short_break_changed,
        app);
    variable_item_set_current_value_index(
        item, app->settings.short_break_minutes - SHORT_BREAK_MIN);
    snprintf(buf, sizeof(buf), "%lum", (unsigned long)app->settings.short_break_minutes);
    variable_item_set_current_value_text(item, buf);

    item = variable_item_list_add(
        app->variable_item_list,
        "Long Break",
        LONG_BREAK_MAX - LONG_BREAK_MIN + 1,
        pomadoro_scene_settings_long_break_changed,
        app);
    variable_item_set_current_value_index(item, app->settings.long_break_minutes - LONG_BREAK_MIN);
    snprintf(buf, sizeof(buf), "%lum", (unsigned long)app->settings.long_break_minutes);
    variable_item_set_current_value_text(item, buf);

    item = variable_item_list_add(
        app->variable_item_list,
        "Rounds",
        ROUNDS_MAX - ROUNDS_MIN + 1,
        pomadoro_scene_settings_rounds_changed,
        app);
    variable_item_set_current_value_index(item, app->settings.rounds - ROUNDS_MIN);
    snprintf(buf, sizeof(buf), "%lu", (unsigned long)app->settings.rounds);
    variable_item_set_current_value_text(item, buf);

    item = variable_item_list_add(
        app->variable_item_list,
        "Vibration",
        PomadoroVibroCount,
        pomadoro_scene_settings_vibro_changed,
        app);
    variable_item_set_current_value_index(item, app->settings.vibro_mode);
    variable_item_set_current_value_text(item, vibro_labels[app->settings.vibro_mode]);

    item = variable_item_list_add(
        app->variable_item_list,
        "Sound",
        PomadoroSoundCount,
        pomadoro_scene_settings_sound_changed,
        app);
    variable_item_set_current_value_index(item, app->settings.sound_mode);
    variable_item_set_current_value_text(item, sound_labels[app->settings.sound_mode]);

    item = variable_item_list_add(app->variable_item_list, "Factory Reset", 1, NULL, app);
    variable_item_set_current_value_index(item, 0);
    variable_item_set_current_value_text(item, "Reset");
}

void pomadoro_scene_settings_on_enter(void* context) {
    PomadoroApp* app = context;

    pomadoro_scene_settings_build_list(app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PomadoroViewVariableItemList);
}

bool pomadoro_scene_settings_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void pomadoro_scene_settings_on_exit(void* context) {
    PomadoroApp* app = context;
    variable_item_list_set_enter_callback(app->variable_item_list, NULL, NULL);
    variable_item_list_reset(app->variable_item_list);
}
