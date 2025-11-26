#pragma once

#include "physics/ball/ball.h"
#include "physics/segment/segment.h"
#include "physics/spring/spring.h"
#include <stddef.h>

typedef struct Body {
    Ball* balls;
    int ballCapacity;
    size_t ballCount;

    Spring* springs;
    int springCapacity;
    size_t springCount;
} Body;

extern long long int collisionChecks;

Body CreateBody();

void AddBallToBody(Body* body, Ball ball);
void AddSpringToBody(Body* body, Spring spring);
void UpdateBody(Body* body, float delta);

void ResolveBodyBodyCollision(Body* bodyA, Body* bodyB);
void ResolveBodySegmentCollision(Body* body, const Segment* segment);

void DrawBody(const Body* body);

Vector2 GetBodyCenter(const Body* body);

Body CreateSoftBodyCircle(Vector2 center, int numBalls, float ringRadius, float ballRadius,
                          Vector2 initialVelocity, float springStiffness,
                          bool fullyConnected, bool addCenter);

Body CreateSoftBodyGrid(Vector2 topLeft, int cols, int rows, float spacing, float ballRadius,
                        Vector2 initialVelocity, float springStiffness,
                        bool connectDiagonals, bool fullyConnected);

Body CreateSoftBodyChain(Vector2 start, Vector2 end, int numBalls, float ballRadius,
                         Vector2 initialVelocity, float springStiffness,
                         bool fullyConnected);