#pragma once
#include <raylib.h>

// Particle structure with basic data
struct Particle {
    Vector2 position;
    Color color;
    float alpha;
    float size;
    float rotation;
    bool active;        // NOTE: Use it to activate/deactive particle
};
