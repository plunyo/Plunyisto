#pragma once

#include "physics/segment/segment.h"
#include <raylib.h>
#include <stddef.h>

typedef struct Body Body;

typedef struct World {
    Body* bodies;
    int bodyCount;
    size_t bodyCapacity;

    Segment* segments;
    int segmentCount;
    size_t segmentCapacity;
} World;

World CreateWorld();

void UpdateWorld(World* world, float delta);
void ResolveWorldCollisions(World* world);
void DrawWorldBodies(const World* world);
void DrawWorldSegments(const World* world);

void AddBodyToWorld(World* world, Body body);

void AddWorldBounds(World* world, Vector2 position, float width, float height);
void AddSegmentToWorld(World* world, Segment segment);