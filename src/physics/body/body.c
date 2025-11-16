#include "physics/body/body.h"
#include "physics/ball/ball.h"
#include <raymath.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

Body CreateBody() {
    Body body;

    body.balls = NULL;
    body.ballCapacity = 0;
    body.ballCount = 0;

    body.springs = NULL;
    body.springCapacity = 0;
    body.springCount = 0;

    return body;
}

void AddBallToBody(Body* body, Ball ball) {
    if (body->ballCount >= body->ballCapacity) {
        size_t newCapacity = body->ballCapacity ? body->ballCapacity * 2 : 4;
        
        body->balls = MemRealloc(body->balls, sizeof(Ball) * newCapacity);
        body->ballCapacity = newCapacity;
    }
    
    body->balls[body->ballCount++] = ball;
}

void AddSpringToBody(Body* body, Spring spring) {
    if (body->springCount >= body->springCapacity) {
        size_t newCapacity = body->springCapacity ? body->springCapacity * 2 : 4;
        
        body->springs = MemRealloc(body->springs, sizeof(Spring) * newCapacity);
        body->springCapacity = newCapacity;
    }
    
    body->springs[body->springCount++] = spring;
}

void UpdateBody(Body* body, float delta) {
    for (size_t i = 0; i < body->ballCount; i++) {
        Ball* ball = &body->balls[i];
        UpdateBall(ball, delta);
    }

    for (size_t i = 0; i < body->springCount; i++) {
        Spring* spring = &body->springs[i];
        ResolveSpring(spring, delta);
    }
}

void ResolveBodyBodyCollision(Body* bodyA, Body* bodyB) {
    for (int i = 0; i < bodyA->ballCount; i++) {
        Ball* ballA = &bodyA->balls[i];

        for (int j = 0; j < bodyB->ballCount; j++) {
            Ball* ballB = &bodyB->balls[j];

            Vector2 difference = Vector2Subtract(ballB->position, ballA->position);
            float distanceSquared = Vector2DotProduct(difference, difference);
            float radiusSum = ballA->radius + ballB->radius;

            if (distanceSquared < radiusSum * radiusSum) {

                float distance = sqrtf(distanceSquared);
                if (distance < 0.0001f) distance = 0.0001f;

                Vector2 normal = Vector2Scale(difference, 1.0f / distance);
                float penetration = radiusSum - distance;

                // position correction (half each because both mass = 1)
                Vector2 correction = Vector2Scale(normal, penetration * 0.5f);

                ballA->position = Vector2Subtract(ballA->position, correction);
                ballB->position = Vector2Add(ballB->position, correction);

                // velocity correction
                Vector2 relativeVel = Vector2Subtract(ballB->velocity, ballA->velocity);
                float velAlongNormal = Vector2DotProduct(relativeVel, normal);

                if (velAlongNormal > 0.0f) continue;

                float restitution = 0.2f; // tweak if your balls bouncing too crazy
                float impulseMag = -(1.0f + restitution) * velAlongNormal * 0.5f; 
                // *0.5f because two masses = 1 each

                Vector2 impulse = Vector2Scale(normal, impulseMag);

                ballA->velocity = Vector2Subtract(ballA->velocity, impulse);
                ballB->velocity = Vector2Add(ballB->velocity, impulse);
            }
        }
    }
}

void ResolveBodySegmentCollision(Body* body, const Segment* segment) {
    Vector2 segDir = Vector2Subtract(segment->end, segment->start);

    for (size_t i = 0; i < body->ballCount; i++) {
        Ball* ball = &body->balls[i];

        Vector2 toBall = Vector2Subtract(ball->position, segment->start);

        float t = Clamp(Vector2DotProduct(toBall, segDir) / Vector2DotProduct(segDir, segDir), 0.0f, 1.0f);

        Vector2 closest = Vector2Add(segment->start, Vector2Scale(segDir, t));

        Vector2 difference = Vector2Subtract(ball->position, closest);
        float distanceSquared = Vector2DotProduct(difference, difference);

        if (distanceSquared < ball->radius * ball->radius) {
            float distance = sqrtf(distanceSquared);
            if (distance == 0.0f) distance = 0.0001f;

            Vector2 collisionNormal = Vector2Scale(difference, 1.0f / distance);
            float penetrationDepth = ball->radius - distance;

            PushBallOut(ball, Vector2Scale(collisionNormal, penetrationDepth));
            ReflectBallVelocity(ball, collisionNormal);
        }
    }
}

void DrawBody(const Body* body) {
    for (size_t i = 0; i < body->springCount; i++) {
        DrawSpring(&body->springs[i]);
    }

    for (size_t i = 0; i < body->ballCount; i++) {
        DrawBall(&body->balls[i]);
    }
}

Vector2 GetBodyCenter(const Body* body) {
    Vector2 center = { 0.0f, 0.0f };

    if (body->ballCount == 0) return center;

    for (size_t i = 0; i < body->ballCount; i++) {
        center = Vector2Add(center, body->balls[i].position);
    }

    center = Vector2Scale(center, 1.0f / (float)body->ballCount);

    return center;
}

static void addSpringBetween(Body *body, Ball *a, Ball *b, float stiffness) {
    Spring s = CreateSpring(a, b, stiffness);
    AddSpringToBody(body, s);
}

Body CreateSoftBodyCircle(Vector2 center, int numBalls, float ringRadius, float ballRadius,
                          Vector2 initialVelocity, float springStiffness,
                          bool fullyConnected, bool addCenter) {

    Body body = CreateBody();
    if (numBalls < 1) return body;

    for (int i = 0; i < numBalls; i++) {
        float ang = (float)i / (float)numBalls * 2.0f * PI;
        Vector2 pos = { center.x + cosf(ang) * ringRadius,
                        center.y + sinf(ang) * ringRadius };

        AddBallToBody(&body, CreateBall(ballRadius, pos, initialVelocity));
    }

    int centerIndex = -1;
    if (addCenter) {
        AddBallToBody(&body, CreateBall(ballRadius, center, initialVelocity));
        centerIndex = body.ballCount - 1;
    }

    if (fullyConnected) {
        for (int i = 0; i < body.ballCount; i++)
            for (int j = i + 1; j < body.ballCount; j++)
                addSpringBetween(&body, &body.balls[i], &body.balls[j], springStiffness);
    } else {
        for (int i = 0; i < numBalls; i++) {
            int j = (i + 1) % numBalls;
            addSpringBetween(&body, &body.balls[i], &body.balls[j], springStiffness);
        }

        if (addCenter) {
            for (int i = 0; i < numBalls; i++)
                addSpringBetween(&body, &body.balls[i], &body.balls[centerIndex], springStiffness);
        }
    }

    return body;
}

Body CreateSoftBodyGrid(Vector2 topLeft, int cols, int rows, float spacing, float ballRadius,
                        Vector2 initialVelocity, float springStiffness,
                        bool connectDiagonals, bool fullyConnected) {

    Body body = CreateBody();
    if (cols < 1 || rows < 1) return body;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Vector2 pos = { topLeft.x + c * spacing,
                            topLeft.y + r * spacing };

            AddBallToBody(&body, CreateBall(ballRadius, pos, initialVelocity));
        }
    }

    if (fullyConnected) {
        for (int i = 0; i < body.ballCount; i++)
            for (int j = i + 1; j < body.ballCount; j++)
                addSpringBetween(&body, &body.balls[i], &body.balls[j], springStiffness);

        return body;
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {

            int i = r * cols + c;

            if (c + 1 < cols)
                addSpringBetween(&body, &body.balls[i], &body.balls[i + 1], springStiffness);

            if (r + 1 < rows)
                addSpringBetween(&body, &body.balls[i], &body.balls[i + cols], springStiffness);

            if (connectDiagonals && r + 1 < rows && c + 1 < cols)
                addSpringBetween(&body, &body.balls[i], &body.balls[i + cols + 1], springStiffness);

            if (connectDiagonals && r + 1 < rows && c - 1 >= 0)
                addSpringBetween(&body, &body.balls[i], &body.balls[i + cols - 1], springStiffness);
        }
    }

    return body;
}

Body CreateSoftBodyChain(Vector2 start, Vector2 end, int numBalls, float ballRadius,
                         Vector2 initialVelocity, float springStiffness,
                         bool fullyConnected) {

    Body body = CreateBody();
    if (numBalls < 1) return body;

    for (int i = 0; i < numBalls; i++) {
        float t = (numBalls == 1 ? 0.0f : (float)i / (float)(numBalls - 1));
        Vector2 pos = { start.x + (end.x - start.x) * t,
                        start.y + (end.y - start.y) * t };

        AddBallToBody(&body, CreateBall(ballRadius, pos, initialVelocity));
    }

    if (fullyConnected) {
        for (int i = 0; i < body.ballCount; i++)
            for (int j = i + 1; j < body.ballCount; j++)
                addSpringBetween(&body, &body.balls[i], &body.balls[j], springStiffness);
    } else {
        for (int i = 0; i < body.ballCount - 1; i++)
            addSpringBetween(&body, &body.balls[i], &body.balls[i + 1], springStiffness);
    }

    return body;
}

Body CreateSoftBodyBlob(Vector2 center, int numBalls, float ringRadius, float ballRadius,
                        Vector2 initialVelocity, float springStiffness) {

    return CreateSoftBodyCircle(center, numBalls, ringRadius, ballRadius,
                                initialVelocity, springStiffness,
                                true, true);
}