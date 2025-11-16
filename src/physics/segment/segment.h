#pragma once

#include "raylib.h"

#define SEGMENT_THICKNESS 1.0f

typedef struct Segment {
    Vector2 start;
    Vector2 end;
} Segment;

Segment CreateSegment(Vector2 start, Vector2 end);
void DrawSegment(const Segment* segment);