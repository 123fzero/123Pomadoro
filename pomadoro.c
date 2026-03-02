#include "pomadoro_app.h"

#include "scenes/pomadoro_scene.h"
#include "views/pomadoro_session_view.h"

static bool pomadoro_custom_event_callback(void* context, uint32_t event) {
    PomadoroApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool pomadoro_back_event_callback(void* context) {
    PomadoroApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static PomadoroApp* pomadoro_alloc(void) {
    PomadoroApp* app = malloc(sizeof(PomadoroApp));

    pomadoro_settings_load(&app->settings);

    app->scene_manager = scene_manager_alloc(&pomadoro_scene_handlers, app);

    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, pomadoro_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, pomadoro_back_event_callback);

    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, PomadoroViewSubmenu, submenu_get_view(app->submenu));

    app->variable_item_list = variable_item_list_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        PomadoroViewVariableItemList,
        variable_item_list_get_view(app->variable_item_list));

    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, PomadoroViewWidget, widget_get_view(app->widget));

    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    app->timer = NULL;

    app->session_view = pomadoro_session_view_alloc();
    view_set_context(app->session_view, app);
    view_dispatcher_add_view(app->view_dispatcher, PomadoroViewSession, app->session_view);

    return app;
}

static void pomadoro_free(PomadoroApp* app) {
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
    }

    view_dispatcher_remove_view(app->view_dispatcher, PomadoroViewSession);
    pomadoro_session_view_free(app->session_view);

    view_dispatcher_remove_view(app->view_dispatcher, PomadoroViewSubmenu);
    submenu_free(app->submenu);

    view_dispatcher_remove_view(app->view_dispatcher, PomadoroViewVariableItemList);
    variable_item_list_free(app->variable_item_list);

    view_dispatcher_remove_view(app->view_dispatcher, PomadoroViewWidget);
    widget_free(app->widget);

    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);

    furi_record_close(RECORD_NOTIFICATION);
    free(app);
}

int32_t pomadoro_app(void* p) {
    UNUSED(p);

    PomadoroApp* app = pomadoro_alloc();
    Gui* gui = furi_record_open(RECORD_GUI);

    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, PomadoroSceneMainMenu);
    view_dispatcher_run(app->view_dispatcher);

    furi_record_close(RECORD_GUI);
    pomadoro_free(app);

    return 0;
}
