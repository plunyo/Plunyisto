#pragma once

#include "physics/ball/ball.h"

#define SPRING_COLOR DARKBLUE
#define SPRING_THICKNESS 5.0f

typedef struct Spring {
    Ball* ballA;
    Ball* ballB;
    float restLength;
    float stiffness;
} Spring;

Spring CreateSpring(Ball* ballA, Ball* ballB, float stiffness);
void ResolveSpring(Spring* spring, float delta);
void DrawSpring(const Spring* spring);
