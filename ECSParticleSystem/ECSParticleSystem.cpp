#include <iostream>
#include <string>
#include <chrono>
#include "ECSSpawnSystem.h"
#include "raylib.h"
#include "SimpleSpawnSystem.h"
#include "PerformanceLogger.h"

enum class AppState
{
    MainMenu,
    ParameterInput,
    RunECS,
    RunArray
};

int main(void)
{
    bool useECS = false;
    InitWindow(1920, 1080, "Particle System");
    SetExitKey(0); // Disable default ESC-to-exit

    Texture2D particleTex = LoadTexture("Assets/Particle-Effect-2.png");

    // Default values, will be set by user input
    int particlesToSpawn = 100000;
    float simulationDuration = 30.0f;

    SimpleSpawnSystem system(particlesToSpawn, particleTex);
    ECSSpawnSystem ecsSystem(particlesToSpawn, particleTex);

    Color messageColor = { 0, 255, 120, 255 };

    PerformanceLogger perfLogger;

    AppState appState = AppState::MainMenu;

    // Button rectangles
    Rectangle btnECS = { 760, 450, 400, 80 };
    Rectangle btnArray = { 760, 570, 400, 80 };

    // Timer variables
    std::chrono::steady_clock::time_point simulationStart;
    bool simulationActive = false;

    // Parameter input state
    std::string inputParticles;
    std::string inputSeconds;
    bool inputParticlesActive = true;
    bool inputSecondsActive = false;
    bool inputReady = false;
    std::string inputErrorMsg;

    // Overlay variables
    std::string overlayMessage;
    bool successMessage = false;
    bool simulationExitedPrematurely = false;
    std::string prematureExitMessage;

	int prematureExitFrameTimer = 0; 

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        char timeLeftText[64];

        if (appState == AppState::MainMenu)
        {
            DrawText("Select Particle System", 800, 300, 40, messageColor);
            DrawText("The simulation will run for the specified seconds and will then automatically return to the Main Menu "
                     "\n \n If you exit the simulation prematurely, its runtime data WILL NOT be saved.", 400, 350, 20, RAYWHITE);

            // Draw buttons
            DrawRectangleRec(btnECS, DARKGRAY);
            DrawRectangleRec(btnArray, DARKGRAY);

            DrawText("Run ECS Particles", (int)btnECS.x + 60, (int)btnECS.y + 25, 32, RAYWHITE);
            DrawText("Run Array Particles", (int)btnArray.x + 40, (int)btnArray.y + 25, 32, RAYWHITE);

            // Button hover effect
            Vector2 mouse = GetMousePosition();

            if (CheckCollisionPointRec(mouse, btnECS))
                DrawRectangleLinesEx(btnECS, 4, GREEN);
            if (CheckCollisionPointRec(mouse, btnArray))
                DrawRectangleLinesEx(btnArray, 4, GREEN);

            // Button click logic
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mouse, btnECS))
                {
                    useECS = true;
                    appState = AppState::ParameterInput;
                    inputParticles.clear();
                    inputSeconds.clear();
                    inputParticlesActive = true;
                    inputSecondsActive = false;
                    inputReady = false;
                }
                else if (CheckCollisionPointRec(mouse, btnArray))
                {
                    useECS = false;
                    appState = AppState::ParameterInput;
                    inputParticles.clear();
                    inputSeconds.clear();
                    inputParticlesActive = true;
                    inputSecondsActive = false;
                    inputReady = false;
                }
            }

            if (successMessage) {
                DrawRectangle(400, 200, 1120, 180, Fade(BLACK, 0.8f));
                DrawText(overlayMessage.c_str(), 420, 240, 28, GREEN);
                DrawText("Press any key or click to dismiss.", 420, 320, 20, RAYWHITE);

                if (GetKeyPressed() != 0 || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    successMessage = false;
                }
            }
            else if (simulationExitedPrematurely) {
                DrawRectangle(400, 200, 1120, 180, Fade(BLACK, 0.8f));
                DrawText(prematureExitMessage.c_str(), 420, 240, 28, RED);
                DrawText("Press any key or click to dismiss.", 420, 320, 20, RAYWHITE);

                prematureExitFrameTimer++; //because we press ESC, we need to wait a few frames before showing the message again
                if (prematureExitFrameTimer > 10 && (GetKeyPressed() != 0 || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
                    simulationExitedPrematurely = false;
                }
            }
        }
        else if (appState == AppState::ParameterInput)
        {
            DrawText("Simulation Parameters", 800, 250, 40, messageColor);

            // Particles input
            DrawText("Particles to Spawn:", 600, 400, 28, RAYWHITE);
            DrawRectangle(900, 395, 300, 40, inputParticlesActive ? LIGHTGRAY : DARKGRAY);
            DrawText(inputParticles.c_str(), 910, 405, 28, BLACK);

            // Seconds input
            DrawText("Seconds of Simulation:", 600, 480, 28, RAYWHITE);
            DrawRectangle(900, 475, 300, 40, inputSecondsActive ? LIGHTGRAY : DARKGRAY);
            DrawText(inputSeconds.c_str(), 910, 485, 28, BLACK);

            DrawText("Press ENTER to confirm each field. After both, simulation will start.", 600, 550, 20, YELLOW);
            DrawText("Press ESC to return to main menu.", 600, 580, 20, messageColor);

            int key = GetCharPressed();
            if (inputParticlesActive)
            {
                // Accept only digits
                while (key > 0)
                {
                    if ((key >= '0' && key <= '9') && inputParticles.length() < 9)
                        inputParticles += (char)key;
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && !inputParticles.empty())
                    inputParticles.pop_back();
                if (IsKeyPressed(KEY_ENTER) && !inputParticles.empty())
                {
                    inputParticlesActive = false;
                    inputSecondsActive = true;
                }
            }
            else if (inputSecondsActive)
            {
                // Accept digits and one dot
                while (key > 0)
                {
                    if ((key >= '0' && key <= '9') && inputSeconds.length() < 6)
                        inputSeconds += (char)key;
                    else if (key == '.' && inputSeconds.find('.') == std::string::npos && !inputSeconds.empty())
                        inputSeconds += '.';
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && !inputSeconds.empty())
                    inputSeconds.pop_back();
                if (IsKeyPressed(KEY_ENTER) && !inputSeconds.empty())
                {
                    inputSecondsActive = false;
                    inputReady = true;
                }
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                appState = AppState::MainMenu;
                continue;
            }

            // When both fields are entered, start simulation
            if (inputReady)
            {
                bool valid = true;
                inputErrorMsg.clear();

                try {
                    particlesToSpawn = std::stoi(inputParticles);
                    simulationDuration = std::stof(inputSeconds);

                    if (particlesToSpawn <= 0) {
                        inputErrorMsg = "Particle count must be greater than 0.";
                        valid = false;
                    }
                    if (simulationDuration <= 0.0f) {
                        inputErrorMsg = "Simulation seconds must be greater than 0.";
                        valid = false;
                    }
                } catch (...) {
                    inputErrorMsg = "Invalid input! Please enter valid numbers.";
                    valid = false;
                }

                if (valid) {
                    // Recreate systems with new particle count
                    system = SimpleSpawnSystem(particlesToSpawn, particleTex);
                    ecsSystem = ECSSpawnSystem(particlesToSpawn, particleTex);

                    perfLogger.SetSimulationTime(simulationDuration);
                    perfLogger.SetParticleCount(particlesToSpawn);

                    simulationStart = std::chrono::steady_clock::now();
                    simulationActive = true;
                    appState = useECS ? AppState::RunECS : AppState::RunArray;
                } else {
                    // Reset input state to let user try again
                    inputParticlesActive = true;
                    inputSecondsActive = false;
                    inputReady = false;
                }
            }

            if (!inputErrorMsg.empty()) {
                DrawText(inputErrorMsg.c_str(), 600, 620, 24, RED);
            }
        }
        else // Simulation running
        {
            float elapsed = 0.0f;
            if (simulationActive) {
                perfLogger.LogFrame(GetFPS(), useECS);
                auto now = std::chrono::steady_clock::now();
                elapsed = std::chrono::duration<float>(now - simulationStart).count();
                float timeLeft = simulationDuration - elapsed;
                if (timeLeft < 0.0f) timeLeft = 0.0f;

                snprintf(timeLeftText, sizeof(timeLeftText), "Time left: %.1f seconds", timeLeft);

                if (elapsed >= simulationDuration) {
                    std::string prefix = useECS ? "ECS_" : "Array_";
                    std::string filename = "performance_log.txt";
                    std::string outDir = "OutputData/";
                    std::string outPath = outDir + prefix + filename;
                    perfLogger.WriteLog(prefix, filename);

                    // Set overlay message for main menu
                    overlayMessage = "The data for the " + std::string(useECS ? "ECS" : "Array") +
                        " simulation was successfully written to:\n" +
                        outPath;
                    successMessage = true;
                    simulationExitedPrematurely = false;

                    // Return to main menu, do not exit
                    appState = AppState::MainMenu;
                    simulationActive = false;
                    continue;
                }
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                appState = AppState::MainMenu;
                simulationActive = false;
                perfLogger = PerformanceLogger(); // Reset logger state
                perfLogger.SetSimulationTime(simulationDuration); // Reset simulation time

                simulationExitedPrematurely = true;
                prematureExitFrameTimer = 0;
                successMessage = false;
                prematureExitMessage = "Simulation was exited prematurely. No data was saved.";

                continue;
            }

            if (appState == AppState::RunArray)
            {
                system.UpdateParticles(10);
                ClearBackground(BLACK);
                system.DrawParticles();
                DrawFPS(10, 10);
                std::string message = "Simple Spawn System\n" + std::to_string(system.GetParticles().size()) + " Particles";
                DrawText(message.c_str(), 20, 60, 20, messageColor);
            }
            else
            {
                ecsSystem.Update(10);
                ClearBackground(Color{ 10, 10, 30, 255 });
                ecsSystem.Draw();
                DrawFPS(10, 10);
                std::string message = "ECS Spawn System\n" + std::to_string(ecsSystem.GetActiveParticleCount()) + " Particles";
                DrawText(message.c_str(), 20, 60, 20, messageColor);
            }

            // Draw UI elements after particles
            DrawText("Press ESC to return to return to Main Menu", 20, 100, 20, messageColor);
            DrawText(timeLeftText, 20, 140, 28, RED);
        }

        EndDrawing();
    }

    UnloadTexture(particleTex);
    CloseWindow();
    return 0;
}