#pragma once

#include <gui/view.h>

#include "../pomadoro_app.h"

typedef struct {
    PomadoroPhase phase;
    uint32_t current_round;
    uint32_t total_rounds;
    uint32_t seconds_remaining;
    uint32_t phase_duration;
    uint32_t total_elapsed;
    bool paused;
} PomadoroSessionModel;

View* pomadoro_session_view_alloc(void);
void pomadoro_session_view_free(View* view);
