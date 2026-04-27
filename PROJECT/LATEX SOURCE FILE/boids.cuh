#pragma once

struct Boid {
    float x, y;
    float vx, vy;
    int exitChoice;  
    unsigned char r, g, b; // NEW: Color data
};

void updateBoidsGPU(Boid* boids, int n, bool reset);