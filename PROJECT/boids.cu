#include "boids.cuh"
#include <cuda_runtime.h>
#include <cmath>

// 🚪 Two exits
#define EXIT1_X 790.0f
#define EXIT1_Y 150.0f
#define EXIT2_X 790.0f
#define EXIT2_Y 450.0f

// 🧱 Wall with two gaps
#define WALL_X 400.0f
#define GAP1_Y1 120.0f
#define GAP1_Y2 180.0f
#define GAP2_Y1 420.0f
#define GAP2_Y2 480.0f

// Fast pseudo-random number generator for GPU noise
__device__ float randomFloat(unsigned int &state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return (state % 1000) / 1000.0f;
}

__global__ void kernel(Boid* boids, int n, int seed) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n) return;

    float x = boids[i].x;
    
    // Skip already exited boids so they don't pile up
    if (x > 5000.0f) return;

    float y = boids[i].y;
    float vx = boids[i].vx;
    float vy = boids[i].vy;
    int exitChoice = boids[i].exitChoice;

    // 🚪 Reached exit - "Delete"
    if (x > 780.0f) {
        boids[i].x = 10000.0f;
        boids[i].y = 10000.0f;
        boids[i].vx = 0;
        boids[i].vy = 0;
        return;
    }

    // 1. Pathfinding (Target gaps first, then the specific exit)
    float targetX, targetY;
    
    // Create a "lane" based on the boid's ID so they distribute across the whole gap width
    float laneOffset = (i % 50) / 50.0f; 
    
    if (x < WALL_X - 10.0f) {
        targetX = WALL_X;
        if (exitChoice == 0) {
            targetY = GAP1_Y1 + 2.0f + laneOffset * (GAP1_Y2 - GAP1_Y1 - 4.0f);
        } else {
            targetY = GAP2_Y1 + 2.0f + laneOffset * (GAP2_Y2 - GAP2_Y1 - 4.0f);
        }
    } else {
        targetX = (exitChoice == 0) ? EXIT1_X : EXIT2_X;
        targetY = (exitChoice == 0) ? EXIT1_Y : EXIT2_Y;
    }

    float dx = targetX - x;
    float dy = targetY - y;
    float dist = sqrtf(dx*dx + dy*dy);
    
    float dirX = 0.0f;
    float dirY = 0.0f;
    if (dist > 0.1f) {
        dirX = dx / dist;
        dirY = dy / dist;
    }

    // 2. Crowd Congestion & Separation
    float sepX = 0.0f;
    float sepY = 0.0f;
    int neighbors = 0;
    
    for (int j = 0; j < n; j++) {
        if (i == j) continue;
        
        float njx = boids[j].x;
        if (njx > 5000.0f) continue;

        float njy = boids[j].y;
        float cdx = x - njx;
        float cdy = y - njy;
        float distSq = cdx*cdx + cdy*cdy;
        
        if (distSq < 36.0f && distSq > 0.01f) { 
            float cdist = sqrtf(distSq);
            float push = (6.0f - cdist);
            sepX += (cdx / cdist) * push * 0.08f;
            sepY += (cdy / cdist) * push * 0.08f;
            neighbors++;
        }
    }

    // 3. Movement Logic
    unsigned int randState = seed + i * 1234567 + (int)x * 98765;
    float noiseX = (randomFloat(randState) - 0.5f) * 0.4f;
    float noiseY = (randomFloat(randState) - 0.5f) * 0.4f;

    // Congestion mechanic: slow down drastically if surrounded
    float targetSpeed = (neighbors > 4) ? 0.3f : 0.8f; 
    if (x >= WALL_X) {
        targetSpeed = 1.2f; 
    }

    float desiredVx = dirX * targetSpeed + sepX + noiseX;
    float desiredVy = dirY * targetSpeed + sepY + noiseY;

    float newVx = vx + (desiredVx - vx) * 0.15f;
    float newVy = vy + (desiredVy - vy) * 0.15f;

    // 4. Wall Collision & Sliding
    float nextX = x + newVx;
    float nextY = y + newVy;

    if (nextX > WALL_X - 5.0f && nextX < WALL_X + 5.0f && x <= WALL_X - 5.0f) {
        bool inGap1 = (nextY > GAP1_Y1 && nextY < GAP1_Y2);
        bool inGap2 = (nextY > GAP2_Y1 && nextY < GAP2_Y2);
        
        if (!inGap1 && !inGap2) {
            newVx = -0.1f; 
            newVy += (targetY > y) ? 0.3f : -0.3f;
        }
    }

    // 5. Apply strict speed limits
    float currentSpeed = sqrtf(newVx*newVx + newVy*newVy);
    float maxSpeed = (x >= WALL_X) ? 1.8f : 1.2f;
    
    if (currentSpeed > maxSpeed) {
        newVx = (newVx / currentSpeed) * maxSpeed;
        newVy = (newVy / currentSpeed) * maxSpeed;
        currentSpeed = maxSpeed;
    }

    // 6. Color Math (Density-based Heatmap)
    // Using the 'neighbors' count we calculated earlier!
    // Let's say 8 or more neighbors is "maximum" density (Pure Red).
    float densityRatio = (float)neighbors / 8.0f; 
    if (densityRatio > 1.0f) densityRatio = 1.0f;

    // Create a smooth transition: Green (Free) -> Yellow (Moderate) -> Red (Stuck)
    if (densityRatio < 0.5f) {
        // Transition from Green (0, 255, 0) to Yellow (255, 255, 0)
        boids[i].r = (unsigned char)(densityRatio * 2.0f * 255.0f);
        boids[i].g = 255;
    } else {
        // Transition from Yellow (255, 255, 0) to Red (255, 0, 0)
        boids[i].r = 255;
        boids[i].g = (unsigned char)((1.0f - densityRatio) * 2.0f * 255.0f);
    }
    boids[i].b = 0; 

    // Update position
    boids[i].x = x + newVx;
    boids[i].y = y + newVy;
    boids[i].vx = newVx;
    boids[i].vy = newVy;
}


void updateBoidsGPU(Boid* h_boids, int n, bool reset) {
    static Boid* d_boids = nullptr;
    static int currentCapacity = 0;
    static int frameCount = 0;

    // Dynamic Memory Allocation
    if (n > currentCapacity) {
        if (d_boids) cudaFree(d_boids);
        cudaMalloc(&d_boids, n * sizeof(Boid));
        currentCapacity = n;
        reset = true; 
    }

    if (reset) {
        cudaMemcpy(d_boids, h_boids, n * sizeof(Boid), cudaMemcpyHostToDevice);
    }

    int threads = 256;
    int blocks = (n + threads - 1) / threads;
    frameCount++;

    kernel<<<blocks, threads>>>(d_boids, n, frameCount);
    cudaDeviceSynchronize();

    cudaMemcpy(h_boids, d_boids, n * sizeof(Boid), cudaMemcpyDeviceToHost);
}