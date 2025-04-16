#include <iostream>
#include <string>

#include "raylib.h"
#include "SimpleSpawnSystem.h"

int main(void)
{
    InitWindow(1920, 1080, "Particle System");

	Texture2D particleTex = LoadTexture("Resources/Particle-Effect-2.png");
	SimpleSpawnSystem system(1000000, particleTex);
	auto particles = system.GetParticles(); //reference
	Color color = { 0, 255, 120, 255 };

    while (!WindowShouldClose()) {
        // Spawn particles at mouse position
        // if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_SPACE)) {
        //     for (int i = 0; i < 100; ++i) {  // Burst spawn
        //         system.SpawnParticle(GetMousePosition());
        //     }
        // }

        // for (int i = 0; i < particles.size(); ++i) {  // Burst spawn
        //     system.SpawnParticle(GetMousePosition());
        // }

        system.UpdateParticles(10);

        BeginDrawing();
        ClearBackground(BLACK);
        system.DrawParticles();
        DrawFPS(10, 10);
        std::string message = std::to_string(particles.size()) + " Particles";
        DrawText(message.c_str(), 20, 60, 20, color);
        EndDrawing();
    }

    UnloadTexture(particleTex);
    CloseWindow();
    return 0;
}