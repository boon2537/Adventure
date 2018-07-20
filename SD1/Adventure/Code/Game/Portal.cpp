#include "Game/Portal.hpp"
#include "Game/GameCommon.hpp"

Portal::~Portal()
{
}

Portal::Portal(const std::string name, PortalDefinition * def, Map* attachedMap)
	: Entity(name, def, attachedMap)
{
	m_portalDef = def;
}

void Portal::DoCollision(Entity* other)
{
	if (other->m_entityDef->m_name == "Player" && !m_justSpawnedPlayer)
	{
		m_linkedPortal->m_justSpawnedPlayer = true;
		other->m_position = m_linkedPortal->m_position;		
		g_theGame->m_currentAdventure->SwitchMap(m_linkedPortal->m_mapAttached->m_name);		
	}
}
