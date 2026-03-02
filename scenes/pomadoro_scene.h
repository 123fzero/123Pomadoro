#pragma once

#include <gui/scene_manager.h>

typedef enum {
    PomadoroSceneMainMenu,
    PomadoroSceneSettings,
    PomadoroSceneSession,
    PomadoroSceneDone,
    PomadoroSceneAbout,
    PomadoroSceneCount,
} PomadoroScene;

typedef enum {
    PomadoroCustomEventMenuSelected,
    PomadoroCustomEventTimerTick,
} PomadoroCustomEvent;

extern const SceneManagerHandlers pomadoro_scene_handlers;
