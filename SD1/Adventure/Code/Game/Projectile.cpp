#include "Game/Projectile.hpp"
#include "Game/GameCommon.hpp"

Projectile::~Projectile()
{
}

Projectile::Projectile(const std::string name, ProjectileDefinition * def, Map* attachedMap, Actor* shooter)
	: Entity(name, def, attachedMap),
	m_shooter(shooter)
{
	m_projectileDef = def;
	m_faction = shooter->m_actorDef->m_faction;
	m_shooterStats = shooter->GetCurrentStats();
	m_colorTint = Stats::GetElementColor(m_shooterStats.m_statValues[STAT_ID::STAT_FIRE_ATK], m_shooterStats.m_statValues[STAT_ID::STAT_WATER_ATK], m_shooterStats.m_statValues[STAT_ID::STAT_EARTH_ATK]);
	m_baseDamage = GetRandomIntInRange(m_projectileDef->m_damageRange.min, m_projectileDef->m_damageRange.max);
}

void Projectile::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	//Apply basic predictive physics
	RayCastResult2D hit = m_mapAttached->RayCast(m_position, m_velocity.GetNormalized(), (m_projectileDef->m_speed * deltaTime) + m_entityDef->m_physicsRadius);
	if (hit.m_didImpact || m_ageInSeconds >= m_projectileDef->m_liveTime)
	{
		Destroy();
	}

	m_position += m_velocity * deltaTime;
}

void Projectile::SetVelocity(float orientation)
{
	m_velocity = Vector2(CosDegrees(orientation), SinDegrees(orientation)) * m_entityDef->m_speed;
}

bool Projectile::CanHit(Actor* actor)
{
	switch (m_projectileDef->m_damageMode)
	{
	case DAMAGE_MODE_OTHER_FACTION:
		return m_faction != actor->m_actorDef->m_faction;
	case DAMAGE_MODE_TARGET_FACTION:
		return m_projectileDef->m_targetFaction == actor->m_actorDef->m_faction;
	case DAMAGE_MODE_ALL:
		if (m_shooter != nullptr)
		{
			return actor != m_shooter;
		}
		return true;
	}

	return false;
}
