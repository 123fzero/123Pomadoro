#include "../pomadoro_app.h"
#include "pomadoro_scene.h"

void pomadoro_scene_main_menu_on_enter(void*);
bool pomadoro_scene_main_menu_on_event(void*, SceneManagerEvent);
void pomadoro_scene_main_menu_on_exit(void*);

void pomadoro_scene_settings_on_enter(void*);
bool pomadoro_scene_settings_on_event(void*, SceneManagerEvent);
void pomadoro_scene_settings_on_exit(void*);

void pomadoro_scene_session_on_enter(void*);
bool pomadoro_scene_session_on_event(void*, SceneManagerEvent);
void pomadoro_scene_session_on_exit(void*);

void pomadoro_scene_done_on_enter(void*);
bool pomadoro_scene_done_on_event(void*, SceneManagerEvent);
void pomadoro_scene_done_on_exit(void*);

void pomadoro_scene_about_on_enter(void*);
bool pomadoro_scene_about_on_event(void*, SceneManagerEvent);
void pomadoro_scene_about_on_exit(void*);

void (*const pomadoro_scene_on_enter_handlers[])(void*) = {
    pomadoro_scene_main_menu_on_enter,
    pomadoro_scene_settings_on_enter,
    pomadoro_scene_session_on_enter,
    pomadoro_scene_done_on_enter,
    pomadoro_scene_about_on_enter,
};

bool (*const pomadoro_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    pomadoro_scene_main_menu_on_event,
    pomadoro_scene_settings_on_event,
    pomadoro_scene_session_on_event,
    pomadoro_scene_done_on_event,
    pomadoro_scene_about_on_event,
};

void (*const pomadoro_scene_on_exit_handlers[])(void*) = {
    pomadoro_scene_main_menu_on_exit,
    pomadoro_scene_settings_on_exit,
    pomadoro_scene_session_on_exit,
    pomadoro_scene_done_on_exit,
    pomadoro_scene_about_on_exit,
};

const SceneManagerHandlers pomadoro_scene_handlers = {
    .on_enter_handlers = pomadoro_scene_on_enter_handlers,
    .on_event_handlers = pomadoro_scene_on_event_handlers,
    .on_exit_handlers = pomadoro_scene_on_exit_handlers,
    .scene_num = PomadoroSceneCount,
};
