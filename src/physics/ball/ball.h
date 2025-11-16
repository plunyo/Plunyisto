#pragma once

#include <raylib.h>

typedef struct Ball {
    float radius;
    Vector2 position;
    Vector2 velocity;
} Ball;

Ball CreateBall(float radius, Vector2 position, Vector2 velocity);
void UpdateBall(Ball* ball, float deltaTime);
void DrawBall(const Ball* ball, Color color);
void ResolveBallCollision(Ball* ball, World* world);
