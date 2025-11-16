#include "physics/world/world.h"
#include "physics/segment/segment.h"
#include "physics/body/body.h"
#include <raylib.h>

World CreateWorld() {
    World world;

    world.bodies = NULL;
    world.bodyCount = 0;
    world.bodyCapacity = 0;

    world.segments = NULL;
    world.segmentCount = 0;
    world.segmentCapacity = 0;

    return world;
}

void UpdateWorld(World* world, float delta) {
    for (int i = 0; i < world->bodyCount; i++) {
        Body* body = &world->bodies[i];
        UpdateBody(body, delta);
    }
}

void ResolveWorldCollisions(World* world) {
    for (size_t i = 0; i < world->bodyCount; i++) {
        Body* bodyA = &world->bodies[i];

        // body v body
        for (size_t j = i + 1; j < world->bodyCount; j++) {
            Body* bodyB = &world->bodies[j];
            ResolveBodyBodyCollision(bodyA, bodyB); // internally checks + handles
        }

        // body v segment
        for (size_t j = 0; j < world->segmentCount; j++) {
            Segment* seg = &world->segments[j];
            ResolveBodySegmentCollision(bodyA, seg); // internally checks + handles
        }
    }
}

void DrawWorldBodies(const World* world) {
    for (int i = 0; i < world->bodyCount; i++) {
        const Body* body = &world->bodies[i];
        DrawBody(body);
    }
}

void DrawWorldSegments(const World* world) {
    for (int i = 0; i < world->segmentCount; i++) {
        const Segment* segment = &world->segments[i];
        DrawSegment(segment);
    }
}

void AddBodyToWorld(World* world, Body body) {
    if (world->bodyCount >= world->bodyCapacity) {
        size_t newCapacity = world->bodyCapacity ? world->bodyCapacity * 2 : 4;
        
        world->bodies = MemRealloc(world->bodies, sizeof(Body) * newCapacity);
        world->bodyCapacity = newCapacity;
    }
    
    world->bodies[world->bodyCount++] = body;
}

void AddSegmentToWorld(World* world, Segment segment) {
    if (world->segmentCount >= world->segmentCapacity) {
        size_t newCapacity = world->segmentCapacity ? world->segmentCapacity * 2 : 4;
        
        world->segments = MemRealloc(world->segments, sizeof(Segment) * newCapacity);
        world->segmentCapacity = newCapacity;
    }
    
    world->segments[world->segmentCount++] = segment;
}

void AddWorldBounds(World* world, Vector2 position, float width, float height) {
    // floor
    AddSegmentToWorld(world, CreateSegment(
        (Vector2){position.x + 0, position.y + height},
        (Vector2){position.x + width, position.y + height}
    ));
    // ceiling
    AddSegmentToWorld(world, CreateSegment(
        (Vector2){position.x + 0, position.y + 0},
        (Vector2){position.x + width, position.y + 0}
    ));
    // left wall
    AddSegmentToWorld(world, CreateSegment(
        (Vector2){position.x + 0, position.y + 0},
        (Vector2){position.x + 0, position.y + height}
    ));
    // right wall
    AddSegmentToWorld(world, CreateSegment(
        (Vector2){position.x + width, position.y + 0},
        (Vector2){position.x + width, position.y + height}
    ));
}