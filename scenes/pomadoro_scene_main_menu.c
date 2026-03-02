#include "../pomadoro_app.h"
#include "pomadoro_scene.h"

enum {
    PomadoroMainMenuIndexStart,
    PomadoroMainMenuIndexSettings,
    PomadoroMainMenuIndexAbout,
};

static void pomadoro_scene_main_menu_callback(void* context, uint32_t index) {
    PomadoroApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void pomadoro_scene_main_menu_on_enter(void* context) {
    PomadoroApp* app = context;

    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "123Pomadoro");
    submenu_add_item(
        app->submenu,
        "Start Cycle",
        PomadoroMainMenuIndexStart,
        pomadoro_scene_main_menu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Settings",
        PomadoroMainMenuIndexSettings,
        pomadoro_scene_main_menu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "About",
        PomadoroMainMenuIndexAbout,
        pomadoro_scene_main_menu_callback,
        app);
    submenu_set_selected_item(
        app->submenu, scene_manager_get_scene_state(app->scene_manager, PomadoroSceneMainMenu));

    view_dispatcher_switch_to_view(app->view_dispatcher, PomadoroViewSubmenu);
}

bool pomadoro_scene_main_menu_on_event(void* context, SceneManagerEvent event) {
    PomadoroApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, PomadoroSceneMainMenu, event.event);

        switch(event.event) {
        case PomadoroMainMenuIndexStart:
            scene_manager_next_scene(app->scene_manager, PomadoroSceneSession);
            consumed = true;
            break;
        case PomadoroMainMenuIndexSettings:
            scene_manager_next_scene(app->scene_manager, PomadoroSceneSettings);
            consumed = true;
            break;
        case PomadoroMainMenuIndexAbout:
            scene_manager_next_scene(app->scene_manager, PomadoroSceneAbout);
            consumed = true;
            break;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);
        consumed = true;
    }

    return consumed;
}

void pomadoro_scene_main_menu_on_exit(void* context) {
    PomadoroApp* app = context;
    submenu_reset(app->submenu);
}
