#pragma once

#include "Game/Entity.hpp"
#include "Game/Actor.hpp"
#include "Game/ProjectileDefinition.hpp"

class Projectile : public Entity
{
public:
	~Projectile();
	Projectile(const std::string name, ProjectileDefinition* def, Map* attachedMap, Actor* shooter);

	virtual void Update(float deltaTime) override;
	void SetVelocity(float orientation);
	bool CanHit(Actor* actor);

public:
	ProjectileDefinition* m_projectileDef = nullptr;
	std::string m_faction;
	Actor* m_shooter = nullptr;
	int m_baseDamage = 0;
	Stats m_shooterStats;
};
