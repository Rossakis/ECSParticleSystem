#include "SimpleSpawnSystem.h"

SimpleSpawnSystem::SimpleSpawnSystem(size_t maxParticles, Texture2D texture) : particleTexture(texture)
{
    spawnArea = { (float)GetScreenWidth() * 1.5f, (float)GetScreenHeight() };

    baseTextureSize = (texture.width + texture.height) / 2.0f;
    particles.resize(maxParticles);

    for (auto& p : particles) {
        ResetParticle(p);
    }
}

void SimpleSpawnSystem::ResetParticle(Particle& p) {
    p.position.x = GetRandomValue(-100, (int)spawnArea.x);// Start slightly off-screen left
    p.position.y = static_cast<float>(GetRandomValue(-500, -50));// Start above screen
    p.color = WHITE;
    p.alpha = GetRandomValue(70, 100) / 100.0f;  // Semi-transparent
    p.size = GetRandomValue(8, 15) / 10.0f;      // Vary raindrop sizes
    p.rotation = 0;
    p.active = true;
}

Particle SimpleSpawnSystem::CreateParticle() {
    return {
        .position = Vector2{0, 0},
        .color = Color{
            static_cast<unsigned char>(GetRandomValue(0, 255)),
            static_cast<unsigned char>(GetRandomValue(0, 255)),
            static_cast<unsigned char>(GetRandomValue(0, 255)),
            255
        },
        .alpha = 1.0f,
        .size = static_cast<float>(GetRandomValue(1, 30)) / 20.0f,
        .rotation = static_cast<float>(GetRandomValue(0, 360)),
        .active = false
    };
}


// Spawn a particle (returns false if no particles available)
bool SimpleSpawnSystem::SpawnParticle(Vector2 position) {
    if (freeIndices.empty()) return false;

    size_t index = freeIndices.back();
    freeIndices.pop_back();

    particles[index] = CreateParticle();
    particles[index].position = position;
    particles[index].active = true;

    return true;
}

void SimpleSpawnSystem::UpdateParticles(float gravity, float decayRate) {
    for (auto& p : particles) {
        p.position.y += gravity * (1.0f + GetRandomValue(-10, 10) / 100.0f);
        p.position.x += 0.3f;

        if (p.position.y > GetScreenHeight() || p.position.x > spawnArea.x) {
            ResetParticle(p);
        }
    }
}

void SimpleSpawnSystem::DrawParticles() const {
    for (const auto& p : particles) {
        if (p.active) {
            float scaledSize = p.size * baseTextureSize;
            Rectangle destRec = {
                p.position.x,
                p.position.y,
                scaledSize,
                scaledSize
            };
            Vector2 origin = { scaledSize / 2, scaledSize / 2 };

            DrawTexturePro(
                particleTexture,
                Rectangle{ 0, 0, (float)particleTexture.width, (float)particleTexture.height },
                destRec,
                origin,
                p.rotation,
                Fade(p.color, p.alpha)
            );
        }
    }
}