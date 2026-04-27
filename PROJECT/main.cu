#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define N 3000   // increase later

struct Boid {
    float x, y;
    float vx, vy;
};

__global__ void updateBoids(Boid* boids, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n) return;

    float sepX = 0, sepY = 0;
    float alignX = 0, alignY = 0;
    float cohX = 0, cohY = 0;

    int count = 0;

    for (int j = 0; j < n; j++) {
        if (i == j) continue;

        float dx = boids[j].x - boids[i].x;
        float dy = boids[j].y - boids[i].y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < 50.0f) {
            sepX -= dx;
            sepY -= dy;

            alignX += boids[j].vx;
            alignY += boids[j].vy;

            cohX += boids[j].x;
            cohY += boids[j].y;

            count++;
        }
    }

    if (count > 0) {
        alignX /= count;
        alignY /= count;

        cohX = (cohX / count) - boids[i].x;
        cohY = (cohY / count) - boids[i].y;
    }

    boids[i].vx += 0.01f * sepX + 0.05f * alignX + 0.01f * cohX;
    boids[i].vy += 0.01f * sepY + 0.05f * alignY + 0.01f * cohY;

    boids[i].x += boids[i].vx;
    boids[i].y += boids[i].vy;

    // wrap screen
    if (boids[i].x < 0) boids[i].x = 800;
    if (boids[i].x > 800) boids[i].x = 0;
    if (boids[i].y < 0) boids[i].y = 600;
    if (boids[i].y > 600) boids[i].y = 0;
}

int main() {
    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "GPU Boids Simulation"
    );

    // Initialize boids
    Boid* h_boids = new Boid[N];
    for (int i = 0; i < N; i++) {
        h_boids[i].x = rand() % 800;
        h_boids[i].y = rand() % 600;
        h_boids[i].vx = 0;
        h_boids[i].vy = 0;
    }

    Boid* d_boids;
    cudaMalloc(&d_boids, N * sizeof(Boid));

    int threads = 256;
    int blocks = (N + threads - 1) / threads;

    sf::CircleShape dot(2);

    while (window.isOpen()) {

        // Events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            window.close();
        }

        // GPU update
        cudaMemcpy(d_boids, h_boids, N * sizeof(Boid), cudaMemcpyHostToDevice);

        updateBoids<<<blocks, threads>>>(d_boids, N);
        cudaDeviceSynchronize();

        cudaMemcpy(h_boids, d_boids, N * sizeof(Boid), cudaMemcpyDeviceToHost);

        // Render
        window.clear();

        for (int i = 0; i < N; i++) {
            dot.setPosition({h_boids[i].x, h_boids[i].y});
            window.draw(dot);
        }

        window.display();
    }

    cudaFree(d_boids);
    delete[] h_boids;

    return 0;
}