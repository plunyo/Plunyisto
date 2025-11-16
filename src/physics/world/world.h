#pragma once

typedef struct World {
    float width;
    float height;
} World;

void DrawWorldBounds(const World* world);