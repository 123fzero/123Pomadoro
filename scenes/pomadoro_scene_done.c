#include "../pomadoro_app.h"
#include "pomadoro_scene.h"

static void pomadoro_scene_done_button_callback(
    GuiButtonType result,
    InputType type,
    void* context) {
    PomadoroApp* app = context;

    if(type == InputTypeShort && result == GuiButtonTypeCenter) {
        view_dispatcher_send_custom_event(app->view_dispatcher, PomadoroCustomEventMenuSelected);
    }
}

void pomadoro_scene_done_on_enter(void* context) {
    PomadoroApp* app = context;

    widget_reset(app->widget);
    widget_add_string_multiline_element(
        app->widget, 64, 20, AlignCenter, AlignCenter, FontPrimary, "Cycle\nComplete!");
    widget_add_button_element(
        app->widget, GuiButtonTypeCenter, "OK", pomadoro_scene_done_button_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PomadoroViewWidget);
}

bool pomadoro_scene_done_on_event(void* context, SceneManagerEvent event) {
    PomadoroApp* app = context;

    if(event.type == SceneManagerEventTypeCustom || event.type == SceneManagerEventTypeBack) {
        scene_manager_search_and_switch_to_previous_scene(
            app->scene_manager, PomadoroSceneMainMenu);
        return true;
    }

    return false;
}

void pomadoro_scene_done_on_exit(void* context) {
    PomadoroApp* app = context;
    widget_reset(app->widget);
}
