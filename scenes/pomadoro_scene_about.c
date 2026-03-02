#include "../pomadoro_app.h"
#include "pomadoro_scene.h"

void pomadoro_scene_about_on_enter(void* context) {
    PomadoroApp* app = context;

    widget_reset(app->widget);
    widget_add_text_scroll_element(
        app->widget,
        0,
        0,
        128,
        64,
        "123Pomadoro v1.1.2\n"
        "Classic focus timer\n"
        "for Flipper Zero\n\n"
        "Flow: focus, short\n"
        "breaks, then a\n"
        "final long break.");

    view_dispatcher_switch_to_view(app->view_dispatcher, PomadoroViewWidget);
}

bool pomadoro_scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void pomadoro_scene_about_on_exit(void* context) {
    PomadoroApp* app = context;
    widget_reset(app->widget);
}
