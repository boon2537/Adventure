#include "Game/AIBehavior_Melee.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/XmlUtilities.hpp"

AIBehavior_Melee::AIBehavior_Melee(const tinyxml2::XMLElement & AIBehaviorXmlElement)
	: AIBehavior(AIBehaviorXmlElement)
{
	m_range = ParseXmlAttribute(AIBehaviorXmlElement, "range", m_range);
	m_attackRate = ParseXmlAttribute(AIBehaviorXmlElement, "attackRate", m_attackRate);
	m_attackDelay = ParseXmlAttribute(AIBehaviorXmlElement, "attackDelay", m_attackDelay);
	m_projectileName = ParseXmlAttribute(AIBehaviorXmlElement, "projectileName", "empty");
	m_meleeOffset = ParseXmlAttribute(AIBehaviorXmlElement, "offSet", Vector2(0, 0));
}

void AIBehavior_Melee::Update(float deltaSeconds)
{	
	AIBehavior::Update(deltaSeconds);

	if (m_nextAttackTriggerTime < m_timeElapsed)
	{
		//dummy velocity for correct animation
		float shootOrientation = Atan2Degress(g_theGame->m_player->m_position.y - m_actorAttached->m_position.y, g_theGame->m_player->m_position.x - m_actorAttached->m_position.x);
		m_actorAttached->m_lastValidVelocity = Vector2(CosDegrees(shootOrientation), SinDegrees(shootOrientation));
		m_actorAttached->SetMeleeAttackDirectionAnimation();
		m_actorAttached->m_directionAtAttack = m_actorAttached->GetMoveDirectionValue(m_actorAttached->m_lastValidVelocity);
		m_actorAttached->m_velocity = Vector2(0, 0);
		m_actorAttached->m_attacking = true;
		m_pendingAttack = true;
		m_nextAttackTriggerTime = m_timeElapsed + (1.f / m_attackRate);
		m_attackTime = m_timeElapsed + m_attackDelay;

		float slowFactor = 0.5f; //TODO: make this set from xml later
		m_actorAttached->m_spriteAnimSet->SetCurrentAnimSpeed(slowFactor);
		m_attackAnimationFinishTime = m_timeElapsed + (m_actorAttached->m_spriteAnimSet->GetCurrentAnimDuration() * (1 / slowFactor));
	}

	if (m_pendingAttack && m_attackTime < m_timeElapsed)
	{
		m_pendingAttack = false;
		Vector2 attackOffset = Vector2(m_meleeOffset.x * m_actorAttached->m_directionAtAttack.x, m_meleeOffset.y * m_actorAttached->m_directionAtAttack.y);
		m_actorAttached->m_mapAttached->SpawnProjectile(m_projectileName, m_actorAttached, m_actorAttached->m_position + attackOffset, m_actorAttached->m_orientation);
	}

	if (m_attackAnimationFinishTime < m_timeElapsed)
	{
		m_actorAttached->m_attacking = false;
	}
}

AIBehavior* AIBehavior_Melee::CloneForActor(Actor * masterActor)
{
	m_actorAttached = masterActor;
	return new AIBehavior_Melee(*this);
}

float AIBehavior_Melee::CalcUtility()
{
	if (m_actorAttached->m_attacking)
	{
		return 1.0f;
	}
	else if (g_theGame->m_player->IsAlive() && GetDistance(m_actorAttached->m_position, g_theGame->m_player->m_position) < m_range)
	{
		return 1.0f;
	}
	else
	{
		return 0;
	}
}
