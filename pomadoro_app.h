#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>
#include <notification/notification_messages.h>

#include "pomadoro_settings.h"

typedef enum {
    PomadoroViewSubmenu,
    PomadoroViewVariableItemList,
    PomadoroViewSession,
    PomadoroViewWidget,
} PomadoroView;

typedef enum {
    PomadoroPhaseFocus,
    PomadoroPhaseShortBreak,
    PomadoroPhaseLongBreak,
} PomadoroPhase;

typedef struct {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    VariableItemList* variable_item_list;
    Widget* widget;
    View* session_view;
    NotificationApp* notifications;
    FuriTimer* timer;
    PomadoroSettings settings;
} PomadoroApp;
