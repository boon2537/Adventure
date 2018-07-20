#include "Game/AIBehavior_Shoot.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/XmlUtilities.hpp"

AIBehavior_Shoot::AIBehavior_Shoot(const tinyxml2::XMLElement & AIBehaviorXmlElement)
	: AIBehavior(AIBehaviorXmlElement)
{
	m_range = ParseXmlAttribute(AIBehaviorXmlElement, "range", m_range);
	m_attackRate = ParseXmlAttribute(AIBehaviorXmlElement, "attackRate", m_attackRate);
	m_attackDelay = ParseXmlAttribute(AIBehaviorXmlElement, "attackDelay", m_attackDelay);
	m_projectileName = ParseXmlAttribute(AIBehaviorXmlElement, "projectileName", "empty");
}

void AIBehavior_Shoot::Update(float deltaSeconds)
{	
	AIBehavior::Update(deltaSeconds);

	if (m_nextAttackTriggerTime < m_timeElapsed)
	{
		//dummy velocity for correct animation
		float shootOrientation = Atan2Degress(g_theGame->m_player->m_position.y - m_actorAttached->m_position.y, g_theGame->m_player->m_position.x - m_actorAttached->m_position.x);
		m_actorAttached->m_lastValidVelocity = Vector2(CosDegrees(shootOrientation), SinDegrees(shootOrientation));
		m_actorAttached->SetCastingDirectionAnimation();
		m_actorAttached->m_velocity = Vector2(0, 0);
		m_actorAttached->m_attacking = true;
		m_pendingAttack = true;
		m_nextAttackTriggerTime = m_timeElapsed + (1.f / m_attackRate);
		m_attackTime = m_timeElapsed + m_attackDelay;
		m_attackAnimationFinishTime = m_timeElapsed + m_actorAttached->m_spriteAnimSet->GetCurrentAnimDuration();
	}

	if (m_pendingAttack && m_attackTime < m_timeElapsed)
	{
		m_pendingAttack = false;
		float shootOrientation = Atan2Degress(g_theGame->m_player->m_position.y - m_actorAttached->m_position.y, g_theGame->m_player->m_position.x - m_actorAttached->m_position.x);
		m_actorAttached->m_mapAttached->SpawnProjectile(m_projectileName, m_actorAttached, m_actorAttached->m_position + (Vector2(CosDegrees(shootOrientation), SinDegrees(shootOrientation)) * 0.3f), shootOrientation);
	}

	if (m_attackAnimationFinishTime < m_timeElapsed)
	{
		m_actorAttached->m_attacking = false;
	}
}

AIBehavior * AIBehavior_Shoot::CloneForActor(Actor * masterActor)
{
	m_actorAttached = masterActor;
	return new AIBehavior_Shoot(*this);
}

float AIBehavior_Shoot::CalcUtility()
{
	if (m_actorAttached->m_attacking)
	{
		return 1.0f;
	}
	else if (g_theGame->m_player->IsAlive() && (GetDistance(m_actorAttached->m_position, g_theGame->m_player->m_position) < m_range)
		&& !m_actorAttached->m_mapAttached->HasLineOfSight(m_actorAttached->m_position, g_theGame->m_player->m_position))
	{
		return 1.0f;
	}
	else
	{
		return 0;
	}
}
