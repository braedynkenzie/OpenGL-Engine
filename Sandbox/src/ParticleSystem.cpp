#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include <random>

class Random
{
public:
	static void Init()
	{
		ENGINE_PROFILE_FUNCTION();

		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		ENGINE_PROFILE_FUNCTION();

		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

ParticleSystem::ParticleSystem(uint32_t maxNumParticles)
	: m_PoolIndex(maxNumParticles - 1)
{
	ENGINE_PROFILE_FUNCTION();

	m_ParticlePool.resize(maxNumParticles);
}

void ParticleSystem::OnUpdate(Engine::Timestep deltaTime)
{
	ENGINE_PROFILE_FUNCTION();

	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= (float)deltaTime;
		particle.Position += particle.Velocity * (float)deltaTime;
		particle.Rotation += 0.01f * (float)deltaTime;
	}
}

void ParticleSystem::OnRender(Engine::OrthographicCamera& orthoCamera)
{
	ENGINE_PROFILE_FUNCTION();

	Engine::Renderer2D::BeginScene(orthoCamera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		float lifeSpan = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 currentParticleColour = glm::lerp(particle.ColourEnd, particle.ColourBegin, lifeSpan);
		currentParticleColour.a = currentParticleColour.a * lifeSpan;
		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, lifeSpan);

		// Render
		Engine::Renderer2D::DrawRotatedQuad({ particle.Position.x, particle.Position.y, 0.0f }, { size, size }, particle.Rotation, currentParticleColour);
	}
	Engine::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProperties& particleProps)
{
	ENGINE_PROFILE_FUNCTION();

	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColourBegin = particleProps.ColourBegin;
	particle.ColourEnd = particleProps.ColourEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}