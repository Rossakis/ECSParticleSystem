#include "ECSSpawnSystem.h"

ECSSpawnSystem::ECSSpawnSystem(size_t maxParticles, Texture2D texture) : particleTexture(texture) {
    spawnArea = { (float)GetScreenWidth() * 1.5f, (float)GetScreenHeight() };
    baseTextureSize = (texture.width + texture.height) / 2.0f;
    InitializeParticles(maxParticles);
}

void ECSSpawnSystem::InitializeParticles(size_t count) {
    for (size_t i = 0; i < count; ++i) {
        auto entity = registry.create();
        ResetParticle(entity);
    }
}

void ECSSpawnSystem::ResetParticle(entt::entity entity) {
    registry.emplace_or_replace<ParticleComponent>(
        entity,
        Vector2{
            (float)GetRandomValue(-100, (int)spawnArea.x),  
            (float)GetRandomValue(-500, -50)               
        },
        Vector2{ 0.3f, 0 },  // Diagonal movement
        WHITE,
        GetRandomValue(70, 100) / 100.0f,  // Alpha
        GetRandomValue(8, 15) / 10.0f,     // Size
        0.0f,                              // Rotation
        true                                // Active
    );
}

void ECSSpawnSystem::Update(float gravity) {
    auto view = registry.view<ParticleComponent>();

    for (auto entity : view) {
        auto& particle = view.get<ParticleComponent>(entity);

        particle.position.y += gravity * (1.0f + GetRandomValue(-10, 10) / 100.0f);
        particle.position.x += particle.velocity.x;

        if (particle.position.y > GetScreenHeight() || particle.position.x > spawnArea.x) {
            ResetParticle(entity);
        }
    }
}

void ECSSpawnSystem::Draw() const {
    auto view = registry.view<const ParticleComponent>();

    BeginBlendMode(BLEND_ALPHA);
    for (auto entity : view) {
        const auto& particle = view.get<const ParticleComponent>(entity);

        if (particle.active) {
            float scaledSize = particle.size * baseTextureSize;
            Rectangle destRec = {
                particle.position.x,
                particle.position.y,
                scaledSize,
                scaledSize
            };

            DrawTexturePro(
                particleTexture,
                { 0, 0, (float)particleTexture.width, (float)particleTexture.height },
                destRec,
                { scaledSize / 2, scaledSize / 2 },
                particle.rotation,
                Fade(particle.color, particle.alpha)
            );
        }
    }
    EndBlendMode();
}