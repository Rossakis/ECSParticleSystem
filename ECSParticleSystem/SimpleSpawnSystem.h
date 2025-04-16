#pragma once

#include <vector>
#include "Particle.h"

class SimpleSpawnSystem
{
	
public:
	SimpleSpawnSystem(size_t maxParticles, Texture2D texture);
	bool SpawnParticle(Vector2 position);
	void UpdateParticles(float gravity = 0.5f, float decayRate = 0.005f);
	const std::vector<Particle>& GetParticles() const { return particles; }
	Particle CreateParticle();
	void DrawParticles() const;
	void ResetParticle(Particle& p);

private:
	std::vector<Particle> particles;
	std::vector<size_t> freeIndices;  // Track available particles for reuse
	Texture2D particleTexture;  // Texture for all particles
	float baseTextureSize;      // Normalization factor for sizing
	Vector2 spawnArea;
};

