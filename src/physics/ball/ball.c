#include "physics/ball/ball.h"
#include "physics/physics.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

Ball* grabbedBall;

Ball CreateBall(float radius, Vector2 position, Vector2 velocity) {
    Ball ball;

    ball.radius = radius;
    ball.position = position;
    ball.velocity = velocity;

    return ball;
}

void UpdateBall(Ball* ball, float delta) {
    if (!ball) return;

    Vector2 mousePos = GetMousePosition();
    float grabRadius = ball->radius * 2.0f;

    // grab logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && grabbedBall == NULL) {
        if (Vector2Distance(mousePos, ball->position) < grabRadius) {
            grabbedBall = ball; // start holding this ball
        }
    }

    // release logic
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && grabbedBall == ball) {
        grabbedBall = NULL;
    }

    if (grabbedBall == ball) {
        ball->position = mousePos;
        ball->velocity = Vector2Zero();
        return;
    }

    // normal physics
    ball->velocity = Vector2Add(ball->velocity, Vector2Scale(GRAVITY, delta));
    float dampingPerSecond = 0.2f;
    float dampingFactor = powf(dampingPerSecond, delta);
    ball->velocity = Vector2Scale(ball->velocity, dampingFactor);
    ball->position = Vector2Add(ball->position, Vector2Scale(ball->velocity, delta));
}

void ReflectBallVelocity(Ball* ball, Vector2 normal) {
    float dot = Vector2DotProduct(ball->velocity, normal);
    ball->velocity = Vector2Scale(Vector2Subtract(ball->velocity, Vector2Scale(normal, 2.0f * dot)), BALL_RESTITUTION);
}

void PushBallOut(Ball* ball, Vector2 correction) {
    ball->position = Vector2Add(ball->position, correction);
}

void DrawBall(const Ball* ball) {
    if (!ball) return;
    DrawCircleV(ball->position, ball->radius, BALL_COLOR);
}
