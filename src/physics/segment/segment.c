#include "physics/segment/segment.h"

Segment CreateSegment(Vector2 start, Vector2 end) {
    Segment segment;

    segment.start = start;
    segment.end = end;

    return segment;
}

void DrawSegment(const Segment* segment) {
    if (!segment) return;
    DrawLineEx(segment->start, segment->end, SEGMENT_THICKNESS, WHITE);
}