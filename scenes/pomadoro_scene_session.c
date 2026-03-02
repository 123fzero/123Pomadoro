#include "../pomadoro_app.h"
#include "../views/pomadoro_session_view.h"
#include "pomadoro_scene.h"

static const NotificationSequence sequence_vibro_short = {
    &message_vibro_on,
    &message_delay_100,
    &message_vibro_off,
    NULL,
};

static const NotificationSequence sequence_vibro_long = {
    &message_vibro_on,
    &message_delay_500,
    &message_vibro_off,
    NULL,
};

static const NotificationSequence sequence_beep = {
    &message_note_c7,
    &message_delay_100,
    &message_sound_off,
    NULL,
};

static uint32_t pomadoro_phase_duration(const PomadoroApp* app, PomadoroPhase phase) {
    switch(phase) {
    case PomadoroPhaseFocus:
        return app->settings.focus_minutes * 60;
    case PomadoroPhaseShortBreak:
        return app->settings.short_break_minutes * 60;
    case PomadoroPhaseLongBreak:
        return app->settings.long_break_minutes * 60;
    default:
        return 0;
    }
}

static void pomadoro_send_alert(PomadoroApp* app) {
    if(app->settings.vibro_mode == PomadoroVibroShort) {
        notification_message(app->notifications, &sequence_vibro_short);
    } else if(app->settings.vibro_mode == PomadoroVibroLong) {
        notification_message(app->notifications, &sequence_vibro_long);
    }

    if(app->settings.sound_mode == PomadoroSoundOn) {
        notification_message(app->notifications, &sequence_beep);
    }
}

static void pomadoro_start_phase(PomadoroApp* app, PomadoroPhase phase) {
    uint32_t duration = pomadoro_phase_duration(app, phase);

    with_view_model(
        app->session_view,
        PomadoroSessionModel * m,
        {
            m->phase = phase;
            m->phase_duration = duration;
            m->seconds_remaining = duration;
            m->paused = false;
        },
        true);

    pomadoro_send_alert(app);
}

static void pomadoro_session_timer_callback(void* context) {
    PomadoroApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PomadoroCustomEventTimerTick);
}

void pomadoro_scene_session_on_enter(void* context) {
    PomadoroApp* app = context;

    with_view_model(
        app->session_view,
        PomadoroSessionModel * m,
        {
            m->phase = PomadoroPhaseFocus;
            m->current_round = 1;
            m->total_rounds = app->settings.rounds;
            m->phase_duration = app->settings.focus_minutes * 60;
            m->seconds_remaining = m->phase_duration;
            m->total_elapsed = 0;
            m->paused = false;
        },
        true);

    pomadoro_send_alert(app);

    app->timer = furi_timer_alloc(pomadoro_session_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, furi_kernel_get_tick_frequency());

    notification_message(app->notifications, &sequence_display_backlight_enforce_on);
    view_dispatcher_switch_to_view(app->view_dispatcher, PomadoroViewSession);
}

bool pomadoro_scene_session_on_event(void* context, SceneManagerEvent event) {
    PomadoroApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom && event.event == PomadoroCustomEventTimerTick) {
        bool session_done = false;
        bool phase_advance = false;
        PomadoroPhase next_phase = PomadoroPhaseFocus;

        with_view_model(
            app->session_view,
            PomadoroSessionModel * m,
            {
                m->total_elapsed++;

                if(m->seconds_remaining > 0) {
                    m->seconds_remaining--;
                }

                if(m->seconds_remaining == 0) {
                    if(m->phase == PomadoroPhaseFocus) {
                        if(m->current_round >= m->total_rounds) {
                            next_phase = PomadoroPhaseLongBreak;
                        } else {
                            next_phase = PomadoroPhaseShortBreak;
                        }
                        phase_advance = true;
                    } else if(m->phase == PomadoroPhaseShortBreak) {
                        m->current_round++;
                        next_phase = PomadoroPhaseFocus;
                        phase_advance = true;
                    } else {
                        session_done = true;
                    }
                }
            },
            true);

        if(session_done) {
            if(app->timer) {
                furi_timer_stop(app->timer);
            }
            notification_message(app->notifications, &sequence_success);
            scene_manager_next_scene(app->scene_manager, PomadoroSceneDone);
        } else if(phase_advance) {
            pomadoro_start_phase(app, next_phase);
        }

        consumed = true;
    } else if(event.type == SceneManagerEventTypeBack) {
        if(app->timer) {
            furi_timer_stop(app->timer);
        }
        scene_manager_search_and_switch_to_previous_scene(
            app->scene_manager, PomadoroSceneMainMenu);
        consumed = true;
    }

    return consumed;
}

void pomadoro_scene_session_on_exit(void* context) {
    PomadoroApp* app = context;

    notification_message(app->notifications, &sequence_display_backlight_enforce_auto);

    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
