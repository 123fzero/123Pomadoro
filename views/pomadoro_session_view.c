#include "pomadoro_session_view.h"

#include "../pomadoro_app.h"
#include <gui/elements.h>
#include <stdio.h>

static const char* pomadoro_phase_label(PomadoroPhase phase) {
    switch(phase) {
    case PomadoroPhaseFocus:
        return "Focus";
    case PomadoroPhaseShortBreak:
        return "Short Break";
    case PomadoroPhaseLongBreak:
        return "Long Break";
    default:
        return "";
    }
}

static void pomadoro_draw_focus_animation(Canvas* canvas, uint32_t elapsed) {
    uint8_t frame = elapsed % 4;
    uint8_t height = 4 + (frame * 2);
    uint8_t y = 50 - height;

    canvas_draw_box(canvas, 18, y, 4, height);
    canvas_draw_box(canvas, 25, y + 2, 4, height - 2);
    canvas_draw_box(canvas, 100, y + 2, 4, height - 2);
    canvas_draw_box(canvas, 107, y, 4, height);
}

static void pomadoro_draw_break_animation(Canvas* canvas, uint32_t elapsed) {
    uint8_t frame = elapsed % 3;

    for(uint8_t i = 0; i < 3; i++) {
        uint8_t x = 54 + (i * 8);
        uint8_t y = (i == frame) ? 41 : 44;
        canvas_draw_box(canvas, x, y, 4, 4);
    }
}

static void pomadoro_session_view_draw(Canvas* canvas, void* model) {
    PomadoroSessionModel* m = model;
    canvas_clear(canvas);

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 10, "123Pomadoro");

    char elapsed[16];
    uint32_t elapsed_min = m->total_elapsed / 60;
    uint32_t elapsed_sec = m->total_elapsed % 60;
    snprintf(
        elapsed,
        sizeof(elapsed),
        "%02lu:%02lu",
        (unsigned long)elapsed_min,
        (unsigned long)elapsed_sec);
    canvas_draw_str_aligned(canvas, 126, 10, AlignRight, AlignBottom, elapsed);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(
        canvas, 64, 27, AlignCenter, AlignBottom, pomadoro_phase_label(m->phase));

    canvas_set_font(canvas, FontSecondary);
    char round_buf[24];
    snprintf(
        round_buf,
        sizeof(round_buf),
        "Round %lu / %lu",
        (unsigned long)m->current_round,
        (unsigned long)m->total_rounds);
    canvas_draw_str_aligned(canvas, 64, 38, AlignCenter, AlignBottom, round_buf);

    char clock_buf[16];
    uint32_t min = m->seconds_remaining / 60;
    uint32_t sec = m->seconds_remaining % 60;
    snprintf(clock_buf, sizeof(clock_buf), "%02lu:%02lu", (unsigned long)min, (unsigned long)sec);
    canvas_draw_str_aligned(canvas, 64, 49, AlignCenter, AlignBottom, clock_buf);

    if(m->phase == PomadoroPhaseFocus) {
        pomadoro_draw_focus_animation(canvas, m->total_elapsed);
    } else {
        pomadoro_draw_break_animation(canvas, m->total_elapsed);
    }

    if(m->phase_duration > 0) {
        uint8_t bar_x = 14;
        uint8_t bar_y = 52;
        uint8_t bar_w = 100;
        uint8_t bar_h = 7;
        uint32_t elapsed_phase = m->phase_duration - m->seconds_remaining;
        uint8_t fill = (uint8_t)(elapsed_phase * (bar_w - 2) / m->phase_duration);
        elements_frame(canvas, bar_x, bar_y, bar_w, bar_h);
        if(fill > 0) {
            canvas_draw_box(canvas, bar_x + 1, bar_y + 1, fill, bar_h - 2);
        }
    }

    if(m->paused) {
        canvas_draw_box(canvas, 3, 56, 2, 6);
        canvas_draw_box(canvas, 7, 56, 2, 6);
    }
}

static bool pomadoro_session_view_input(InputEvent* event, void* context) {
    PomadoroApp* app = context;

    if(event->type == InputTypeShort && event->key == InputKeyOk) {
        bool paused = false;
        with_view_model(
            app->session_view,
            PomadoroSessionModel * m,
            {
                m->paused = !m->paused;
                paused = m->paused;
            },
            true);

        if(paused) {
            if(app->timer) {
                furi_timer_stop(app->timer);
            }
        } else {
            if(app->timer) {
                furi_timer_start(app->timer, furi_kernel_get_tick_frequency());
            }
        }
        return true;
    }

    return false;
}

View* pomadoro_session_view_alloc(void) {
    View* view = view_alloc();
    view_allocate_model(view, ViewModelTypeLocking, sizeof(PomadoroSessionModel));
    view_set_draw_callback(view, pomadoro_session_view_draw);
    view_set_input_callback(view, pomadoro_session_view_input);
    return view;
}

void pomadoro_session_view_free(View* view) {
    view_free(view);
}
