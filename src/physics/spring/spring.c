#include "physics/spring/spring.h"

#include <raymath.h>

Spring CreateSpring(Ball* ballA, Ball* ballB, float stiffness) {
    Spring spring;

    spring.ballA = ballA;
    spring.ballB = ballB;
    spring.restLength = Vector2Distance(ballA->position, ballB->position);
    spring.stiffness = stiffness;

    return spring;
}

void ResolveSpring(Spring* spring, float delta) {
    Ball* ballA = spring->ballA;
    Ball* ballB = spring->ballB;

    Vector2 deltaPos = Vector2Subtract(ballB->position, ballA->position);
    float currentLength = Vector2Length(deltaPos);
    float displacement = currentLength - spring->restLength;

    Vector2 direction = Vector2Normalize(deltaPos);
    Vector2 force = Vector2Scale(direction, spring->stiffness * displacement);

    Vector2 accelerationA = force; 
    Vector2 accelerationB = Vector2Negate(force);

    ballA->velocity = Vector2Add(ballA->velocity, Vector2Scale(accelerationA, delta));
    ballB->velocity = Vector2Add(ballB->velocity, Vector2Scale(accelerationB, delta));
}

void DrawSpring(const Spring* spring) {
    if (!spring) return;
    DrawLineEx(spring->ballA->position, spring->ballB->position, SPRING_THICKNESS, SPRING_COLOR);
}
