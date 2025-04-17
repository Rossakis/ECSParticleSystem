#pragma once
#include <entt/entt.hpp>
#include "Particle.h"
#include <raylib.h>

struct ParticleComponent {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float alpha;
    float size;
    float rotation;
    bool active;
};

class ECSSpawnSystem
{   
public:
    ECSSpawnSystem(size_t maxParticles, Texture2D texture);
    void Update(float gravity = 0.5f);
    void Draw() const;
    void ResetParticle(entt::entity entity);
	size_t GetActiveParticleCount() const {
        return registry.view<const ParticleComponent>().size();
	}
private:
    entt::registry registry;
    Texture2D particleTexture;
    float baseTextureSize;
    Vector2 spawnArea;

    void InitializeParticles(size_t count);
};

