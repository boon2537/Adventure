#pragma once

#include "Game/Entity.hpp"
#include "Game/PortalDefinition.hpp"

class Portal : public Entity
{
public:
	~Portal();
	Portal(const std::string name, PortalDefinition* def, Map* attachedMap);
	virtual void DoCollision(Entity* other) override;

public:
	PortalDefinition* m_portalDef = nullptr;
	Portal* m_linkedPortal = nullptr;
	bool m_justSpawnedPlayer = false;
};
