#pragma once

#include <raylib.h>

#define BALL_COLOR RAYWHITE
#define BALL_RESTITUTION 0.99f


typedef struct Ball {
    float radius;
    Vector2 position;   
    Vector2 velocity;
} Ball;

Ball CreateBall(float radius, Vector2 position, Vector2 velocity);
void UpdateBall(Ball* ball, float delta);
void ReflectBallVelocity(Ball* ball, Vector2 normal);
void PushBallOut(Ball* ball, Vector2 correction);
void DrawBall(const Ball* ball);

extern Ball* grabbedBall;