#include "physics/ball/ball.h"
#include "physics/physics.h"
#include <raymath.h>

Ball CreateBall(float radius, Vector2 position, Vector2 velocity) {
    Ball ball;

    ball.radius = radius;
    ball.position = position;
    ball.velocity = velocity;

    return ball;
}

void UpdateBall(Ball* ball, float delta) {
    if (!ball) return;

    ball->velocity = Vector2Add(ball->velocity, Vector2Scale(GRAVITY, delta));

    float dampingPerSecond = 0.5f;
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
