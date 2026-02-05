#pragma once

#include <gui/view.h>

typedef struct SplashView SplashView;

SplashView *splash_view_alloc();

void splash_view_free(SplashView *instance);

View *splash_view_get_view(SplashView *instance);

void splash_view_timer_tick(SplashView *instance);
