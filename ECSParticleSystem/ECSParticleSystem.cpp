#include <iostream>
#include <string>

#include "ECSSpawnSystem.h"
#include "raylib.h"
#include "SimpleSpawnSystem.h"

int main(void)
{
	bool useECS = false;
    InitWindow(1920, 1080, "Particle System");

	Texture2D particleTex = LoadTexture("Resources/Particle-Effect-2.png");

    //Init both systems
	SimpleSpawnSystem system(100000, particleTex);
    ECSSpawnSystem ecsSystem(100000, particleTex);

	Color messageColor = { 0, 255, 120, 255 };

    while (!WindowShouldClose()) {

        DrawText("Press Space to change from Simple to ECS spawn system", 20, 60, 20, messageColor);

        if(IsKeyPressed(KEY_SPACE))
			useECS = !useECS;

        if(!useECS)
        {
            system.UpdateParticles(10);

            BeginDrawing();
            ClearBackground(BLACK);
            system.DrawParticles();
            DrawFPS(10, 10);
            std::string message = "Simple Spawn System\n" + std::to_string(system.GetParticles().size()) + " Particles";
            DrawText(message.c_str(), 20, 60, 20, messageColor);
            EndDrawing();
        }
        else
        {
            ecsSystem.Update(10); // Slightly heavier rain

            BeginDrawing();
            ClearBackground(Color{ 10, 10, 30, 255 });
            ecsSystem.Draw();
            DrawFPS(10, 10);
            std::string message = "ECS Spawn System\n" + std::to_string(ecsSystem.GetActiveParticleCount()) + " Particles";
            DrawText(message.c_str(), 20, 60, 20, messageColor);
            EndDrawing();
        }
    }

    UnloadTexture(particleTex);
    CloseWindow();
    return 0;
}