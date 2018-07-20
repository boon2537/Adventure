#include "Game/Map.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Vertex.hpp"
#include <queue>

Map::Map(const std::string& name, MapDefinition * def)
	: m_name(name)
	, m_mapDefinition(def)
{
	m_mapWidth = GetRandomIntInRange(def->m_widthRange.min, def->m_widthRange.max);
	m_mapHeight = GetRandomIntInRange(def->m_heightRange.min, def->m_heightRange.max);

	//Set default tile
	for (int y = 0; y < m_mapHeight; y++)
	{
		for (int x = 0; x < m_mapWidth; x++)
			m_tiles.push_back(Tile(IntVector2(x, y), def->m_defaultTile));
	}

	//Run generation steps
	for (unsigned int i = 0; i < m_mapDefinition->m_MapGenSteps.size(); i++)
	{
		MapGenStep& step = *m_mapDefinition->m_MapGenSteps[i];
		step.Generate(*this);
	}

	m_playerDistanceField = HeatMap(IntVector2(m_mapWidth, m_mapHeight), HEAT_MAP_FILL_VALUE);
	m_playerCoord.push_back(GetTile(0, 0).m_tileCoords);
}

void Map::Update(float deltaTime)
{
	//Update entities
	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i] != nullptr)
			m_entities[i]->Update(deltaTime);
	}

	//Projectile vs Actor physics
	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		Projectile* projectile = m_projectiles[i];
		if (projectile != nullptr)
		{
			for (unsigned int j = 0; j < m_actors.size(); j++)
			{
				Actor* actor = m_actors[j];
				if (actor != nullptr && projectile->CanHit(actor)
					&& DoDiscsOverlap(projectile->m_position, projectile->m_entityDef->m_physicsRadius, actor->m_position, actor->m_entityDef->m_physicsRadius))
				{
					actor->TakeDamage(projectile->m_baseDamage, projectile->m_shooterStats);
					projectile->Destroy();
				}
			}
		}
	}

	//Actor vs Actor physics
	for (unsigned int index = 0; index < m_actors.size(); index++)
	{
		for (unsigned int againstIndex = 0; againstIndex < m_actors.size(); againstIndex++)
		{
			Actor* target = m_actors[index];
			Actor* against = m_actors[againstIndex];
			if (target != against && target != nullptr && !target->ShouldBeDestroyed() && target->m_actorDef->m_enableActorVsActorPhysics 
				&& against != nullptr && !against->ShouldBeDestroyed() && against->m_actorDef->m_enableActorVsActorPhysics &&
				DoDiscsOverlap(target->m_position, target->m_entityDef->m_physicsRadius, against->m_position, against->m_entityDef->m_physicsRadius))
			{
				float pushBackMagnitude = GetDiscOverlapMagnitude(target->m_position, target->m_entityDef->m_physicsRadius, against->m_position, against->m_entityDef->m_physicsRadius);
				float targetMassFactor = against->m_actorDef->m_mass / (target->m_actorDef->m_mass + against->m_actorDef->m_mass);
				float againstMassFactor = target->m_actorDef->m_mass / (target->m_actorDef->m_mass + against->m_actorDef->m_mass);
				target->m_position = target->m_position + ((target->m_position - against->m_position) * pushBackMagnitude * targetMassFactor);
				against->m_position = against->m_position + ((against->m_position - target->m_position) * pushBackMagnitude * againstMassFactor);
			}
		}
	}

	//Portal vs Player
	for (unsigned int i = 0; i < m_portals.size(); i++)
	{
		Portal* portal = m_portals[i];
		if (portal != nullptr)
		{
			if (DoDiscsOverlap(portal->m_position, portal->m_entityDef->m_physicsRadius, g_theGame->m_player->m_position, g_theGame->m_player->m_entityDef->m_physicsRadius))
				portal->DoCollision(g_theGame->m_player);
			else //like onCollider exit
				portal->m_justSpawnedPlayer = false;
		}
	}

	//Dropped Item vs Player
	for (unsigned int i = 0; i < m_itemsOnGround.size(); i++)
	{
		Item* item = m_itemsOnGround[i];
		if (item != nullptr)
		{
			if (DoDiscsOverlap(item->m_position, item->m_entityDef->m_physicsRadius, g_theGame->m_player->m_position, g_theGame->m_player->m_entityDef->m_physicsRadius))
				g_theGame->m_player->m_itemToPickUp = item;
			else //like onCollider exit
			{
				if (g_theGame->m_player->m_itemToPickUp == item)
					g_theGame->m_player->m_itemToPickUp = nullptr;
			}
		}
	}

	//Apply corrective physics
	for (unsigned int i = 0; i < m_actors.size(); i++)
	{
		Actor* actor = m_actors[i];
		if (actor != nullptr)
		{
			Tile myTile = GetClosetTileFromPosition(actor->m_position);
			float entityPhysicsRadius = actor->m_entityDef->m_physicsRadius;

			Tile tileToCheck = GetTile(myTile.m_tileCoords.x, myTile.m_tileCoords.y + 1); // GetTopTile
			if (!CanEntityEnterTile(tileToCheck, actor) && DoDiscToPointOverlap(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)))
				actor->m_position.y += GetDiscPushOutOfPointFactor(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)).y;
			tileToCheck = GetTile(myTile.m_tileCoords.x, myTile.m_tileCoords.y - 1); //GetBotTile
			if (!CanEntityEnterTile(tileToCheck, actor) && DoDiscToPointOverlap(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)))
				actor->m_position.y += GetDiscPushOutOfPointFactor(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)).y;
			tileToCheck = GetTile(myTile.m_tileCoords.x - 1, myTile.m_tileCoords.y); //GetLeftTile
			if (!CanEntityEnterTile(tileToCheck, actor) && DoDiscToPointOverlap(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)))
				actor->m_position.x += GetDiscPushOutOfPointFactor(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)).x;
			tileToCheck = GetTile(myTile.m_tileCoords.x + 1, myTile.m_tileCoords.y); //GetRightTile
			if (!CanEntityEnterTile(tileToCheck, actor) && DoDiscToPointOverlap(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)))
				actor->m_position.x += GetDiscPushOutOfPointFactor(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)).x;

			tileToCheck = GetTile(myTile.m_tileCoords.x + 1, myTile.m_tileCoords.y + 1); //GetTopRightTile
			if (!CanEntityEnterTile(tileToCheck, actor) && DoDiscToPointOverlap(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)))
				actor->m_position += GetDiscPushOutOfPointFactor(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck));
			tileToCheck = GetTile(myTile.m_tileCoords.x - 1, myTile.m_tileCoords.y + 1); //GetTopLeftTile
			if (!CanEntityEnterTile(tileToCheck, actor) && DoDiscToPointOverlap(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)))
				actor->m_position += GetDiscPushOutOfPointFactor(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck));
			tileToCheck = GetTile(myTile.m_tileCoords.x + 1, myTile.m_tileCoords.y - 1); //GetBotRightTile
			if (!CanEntityEnterTile(tileToCheck, actor) && DoDiscToPointOverlap(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)))
				actor->m_position += GetDiscPushOutOfPointFactor(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck));
			tileToCheck = GetTile(myTile.m_tileCoords.x - 1, myTile.m_tileCoords.y - 1); //GetBotLeftTile
			if (!CanEntityEnterTile(tileToCheck, actor) && DoDiscToPointOverlap(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck)))
				actor->m_position += GetDiscPushOutOfPointFactor(actor->m_position, entityPhysicsRadius, GetClosestPointToTile(actor->m_position, tileToCheck));
		}
	}

	//Update Player Distance Field
	if (g_theGame->m_player->m_mapAttached == this)
	{
		m_playerCoord[0] = GetClosetTileFromPosition(g_theGame->m_player->m_position).m_tileCoords;
		UpdateDistanceField(m_playerDistanceField, m_playerCoord);
	}

	// Entity deletions & cleanups
	for (unsigned int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] != nullptr && m_actors[i]->ShouldBeDestroyed())
		{
			if (m_actors[i] == g_theGame->m_player->m_itemTransferTarget)
				g_theGame->m_player->m_itemTransferTarget = nullptr;

			m_actors[i] = nullptr;
		}
	}
	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles[i] != nullptr && m_projectiles[i]->ShouldBeDestroyed())
			m_projectiles[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_portals.size(); i++)
	{
		if (m_portals[i] != nullptr && m_portals[i]->ShouldBeDestroyed())
			m_portals[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_itemsOnGround.size(); i++)
	{
		if (m_itemsOnGround[i] != nullptr && m_itemsOnGround[i]->ShouldBeDestroyed())
			m_itemsOnGround[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i] != nullptr && m_entities[i]->ShouldBeDestroyed())
		{
			m_entities[i]->OnDestroy();
			delete m_entities[i];
			m_entities[i] = nullptr;
		}
	}

	//Entity sorted draw order
	for (unsigned int i = 0; i < m_entities.size() - 1; i++)
	{
		Entity* first = m_entities[i];
		Entity* second = m_entities[i + 1];
		if (first == nullptr || (first != nullptr && second != nullptr && first->m_position.y < second->m_position.y))
		{
			m_entities[i] = second;
			m_entities[i + 1] = first;
		}
	}
}

void Map::LateUpdate(float deltaTime)
{
	//LateUpdate entities
	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i] != nullptr)
			m_entities[i]->LateUpdate(deltaTime);
	}
}

void Map::Render()
{
	// Render Tiles
	static std::vector<Vertex3D_PCU> tileVerts;
	tileVerts.clear();
	for (unsigned int i = 0; i < m_tiles.size(); i++)
	{
		AABB2 worldBounds = m_tiles[i].GetWorldBounds();
		if (g_theGame->m_camera->IsBoundsInView(worldBounds))
		{
			AABB2 texCoords = m_tiles[i].m_tileDef->m_baseTextureCoords;
			tileVerts.push_back(Vertex3D_PCU(Vector2(worldBounds.mins.x, worldBounds.maxs.y), m_tiles[i].m_tileDef->m_baseSpriteTint, Vector2(texCoords.mins.x, texCoords.mins.y)));
			tileVerts.push_back(Vertex3D_PCU(Vector2(worldBounds.mins.x, worldBounds.mins.y), m_tiles[i].m_tileDef->m_baseSpriteTint, Vector2(texCoords.mins.x, texCoords.maxs.y)));
			tileVerts.push_back(Vertex3D_PCU(Vector2(worldBounds.maxs.x, worldBounds.mins.y), m_tiles[i].m_tileDef->m_baseSpriteTint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
			tileVerts.push_back(Vertex3D_PCU(Vector2(worldBounds.maxs.x, worldBounds.maxs.y), m_tiles[i].m_tileDef->m_baseSpriteTint, Vector2(texCoords.maxs.x, texCoords.mins.y)));

			if (m_tiles[i].m_tileDef->m_hasOverlayTexture)
			{
				texCoords = m_tiles[i].m_tileDef->m_overlayTextureCoords;
				tileVerts.push_back(Vertex3D_PCU(Vector2(worldBounds.mins.x, worldBounds.maxs.y), m_tiles[i].m_tileDef->m_overlaySpriteTint, Vector2(texCoords.mins.x, texCoords.mins.y)));
				tileVerts.push_back(Vertex3D_PCU(Vector2(worldBounds.mins.x, worldBounds.mins.y), m_tiles[i].m_tileDef->m_overlaySpriteTint, Vector2(texCoords.mins.x, texCoords.maxs.y)));
				tileVerts.push_back(Vertex3D_PCU(Vector2(worldBounds.maxs.x, worldBounds.mins.y), m_tiles[i].m_tileDef->m_overlaySpriteTint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
				tileVerts.push_back(Vertex3D_PCU(Vector2(worldBounds.maxs.x, worldBounds.maxs.y), m_tiles[i].m_tileDef->m_overlaySpriteTint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
			}
		}
	}
	g_theRenderer->DrawTextureFromVertexArray(tileVerts.data(), tileVerts.size(), g_terrainSpriteSheet->GetTexture());

	// Render Entities
	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i] != nullptr)
		{
			m_entities[i]->Render();
			if (g_theGame->DEBUG_RENDER)
				m_entities[i]->RenderDebug();
		}
	}

	if (g_theGame->DEBUG_RENDER)
	{
		for (int y = 0; y < m_mapHeight; y++)
		{
			for (int x = 0; x < m_mapWidth; x++)
			{
				Tile t = GetTile(x, y);
				g_theRenderer->DrawTextInBox2D(AABB2((float)t.m_tileCoords.x, (float)t.m_tileCoords.y, (float)t.m_tileCoords.x + 1, (float)t.m_tileCoords.y + 1), std::to_string((int) m_playerDistanceField.GetHeat(t.m_tileCoords)), 0.2f);
			}
		}
	}
}

bool Map::CanGetTile(int x, int y)
{
	if (x < 0 || y < 0 || x >= m_mapWidth || y >= m_mapHeight)
	{
		return false;
	}
	return true;
}

Tile& Map::GetTile(int x, int y)
{
	unsigned int index = x + (y * m_mapWidth);
	if (index < m_tiles.size())
		return m_tiles[index];

	ERROR_AND_DIE("Tile out of range");
}

Tile& Map::GetClosetTileFromPosition(const Vector2& position)
{
	return GetTile(static_cast<int>(truncf(position.x)), static_cast<int>(truncf(position.y)));
}

Tile* Map::GetRandomTileOfDef(const TileDefinition * def)
{
	std::vector<Tile*> tiles = GetTilesOfDef(def);
	if (tiles.size() == 0)
		return nullptr;

	return tiles[GetRandomIntLessThan(tiles.size())];
}

std::vector<Tile*> Map::GetTilesOfDef(const TileDefinition * def)
{
	std::vector<Tile*> result = std::vector<Tile*>();
	for (int y = 0; y < m_mapHeight; y++)
	{
		for (int x = 0; x < m_mapWidth; x++)
		{
			if (GetTile(x, y).m_tileDef == def)
				result.push_back(&GetTile(x, y));
		}
	}
	return result;
}

Vector2 Map::GetClosestPointToTile(const Vector2 &center, Tile &tile)
{
	float closestX = ClampFloat((float) center.x, (float) tile.m_tileCoords.x, (float) tile.m_tileCoords.x + 1);
	float closestY = ClampFloat((float)center.y, (float) tile.m_tileCoords.y, (float) tile.m_tileCoords.y + 1);
	return Vector2(closestX, closestY);
}

bool Map::CanEntityEnterTile(const Tile tile, const Entity * entity)
{
	return (tile.m_tileDef->m_allowsWalking && entity->m_entityDef->canWalk)
		|| (tile.m_tileDef->m_allowsFlying && entity->m_entityDef->canSwim)
		|| (tile.m_tileDef->m_allowsSwimming && entity->m_entityDef->canFly);
}

Projectile* Map::SpawnProjectile(const std::string& typeName, Actor* shooter, const Vector2& pos, float orientation)
{
	Projectile* projectile = new Projectile(typeName, ProjectileDefinition::GetProjectileDefinition(typeName), this, shooter);
	projectile->m_position = pos;
	projectile->m_orientation = orientation;
	projectile->SetVelocity(orientation);

	m_projectiles.push_back(projectile);
	m_entities.push_back(projectile);
	return projectile;
}

Actor* Map::SpawnActor(const std::string& typeName, const Vector2& pos, float orientation)
{
	Actor* actor = new Actor(typeName, ActorDefinition::GetActorDefinition(typeName), this);
	actor->m_position = pos;
	actor->m_orientation = orientation;

	m_actors.push_back(actor);
	m_entities.push_back(actor);
	return actor;
}

Portal* Map::SpawnPortal(const std::string& typeName, const Vector2& pos, float orientation)
{
	Portal* portal = new Portal(typeName, PortalDefinition::GetPortalDefinition(typeName), this);
	portal->m_position = pos;
	portal->m_orientation = orientation;

	m_portals.push_back(portal);
	m_entities.push_back(portal);
	return portal;
}

Item* Map::SpawnItem(const std::string& typeName, const Vector2& pos, float orientation)
{
	Item* item = new Item(typeName, ItemDefinition::GetItemDefinition(typeName), this);
	item->m_position = pos;
	item->m_orientation = orientation;

	m_itemsOnGround.push_back(item);
	m_entities.push_back(item);
	return item;
}

void Map::ReceiveItem(Item* item, const Vector2& pos, float orientation)
{
	item->m_position = pos;
	item->m_orientation = orientation;
	item->m_mapAttached = this;
	m_itemsOnGround.push_back(item);
	m_entities.push_back(item);
}

void Map::RemovePickedUpItem(Item* item)
{
	for (unsigned int i = 0; i < m_itemsOnGround.size(); i++)
	{
		if (m_itemsOnGround[i] == item)
			m_itemsOnGround[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i] == item)
			m_entities[i] = nullptr;
	}
}

void Map::AttachPlayerToMap(Player* player)
{
	player->m_mapAttached = this;
	m_actors.push_back(player);
	m_entities.push_back(player);
}

void Map::DeattachPlayerFromMap(Player* player)
{
	player->m_mapAttached = nullptr;
	for (unsigned int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] != nullptr && m_actors[i] == player)
			m_actors[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i] != nullptr && m_entities[i] == player)
			m_entities[i] = nullptr;
	}
}

bool Map::HasLineOfSight(const Vector2 & startPos, const Vector2 & endPos)
{
	Vector2 direction = endPos - startPos;
	float distance = direction.NormalizeAndGetLength();
	return RayCast(startPos, direction, distance).m_didImpact;
}

RayCastResult2D Map::RayCast(const Vector2 & start, const Vector2 & direction, float distance)
{
	int numSteps = static_cast<int>(distance *  RAYCAST_STEPS_PER_TILE);
	Vector2 singleStep = direction * static_cast<float>(1.f / RAYCAST_STEPS_PER_TILE);
	Tile currentTile = GetClosetTileFromPosition(start);
	Tile previousTile = currentTile;

	if (!currentTile.m_tileDef->m_allowsWalking)
		return RayCastResult2D();

	for (int stepNumber = 0; stepNumber < numSteps; stepNumber++)
	{
		Vector2 currentPosition = start + (singleStep * (float) stepNumber);
		currentTile = GetClosetTileFromPosition(currentPosition);

		if (currentTile.m_tileCoords == previousTile.m_tileCoords)
			continue; //nothing new of interest to check... continue looping

		if (!currentTile.m_tileDef->m_allowsWalking)
		{
			float distanceHit = GetDistance(currentPosition, start);
			IntVector2 normal = previousTile.m_tileCoords - currentTile.m_tileCoords;
			return RayCastResult2D(true, currentPosition, currentTile.m_tileCoords, distanceHit, distanceHit / distance, Vector2(static_cast<float>(normal.x), static_cast<float>(normal.y))); 
		}

		previousTile = currentTile;
	}

	return RayCastResult2D(); //doesn't hit anything
}

void Map::UpdateDistanceField(HeatMap& heatMap, const std::vector<IntVector2>& seedCoords)
{
	heatMap.InitHeat(HEAT_MAP_FILL_VALUE);

	std::queue<IntVector2> openList;
	for (unsigned int i = 0; i < seedCoords.size(); i++)
	{
		heatMap.SetHeat(seedCoords[i], 0);
		openList.push(seedCoords[i]);
	}

	while (!openList.empty())
	{
		IntVector2 coordToUse = openList.front();
		openList.pop();
		float initValue = heatMap.GetHeat(coordToUse);	
		float lookAtValue;

		//top
		if (CanGetTile(coordToUse.x, coordToUse.y + 1))
		{
			Tile tileToCheck = GetTile(coordToUse.x, coordToUse.y + 1);
			float valueToAdd = 1;
			lookAtValue = heatMap.GetHeat(tileToCheck.m_tileCoords);
			//TODO: implement proper impassable later for flying, etc.
			if (!tileToCheck.m_tileDef->m_allowsWalking)
				valueToAdd = HEAT_MAP_IMPASSABLE_VALUE + 1;
			if (initValue + valueToAdd < lookAtValue)
			{
				heatMap.SetHeat(tileToCheck.m_tileCoords, initValue + valueToAdd);
				openList.push(tileToCheck.m_tileCoords);
			}
		}
		//bot
		if (CanGetTile(coordToUse.x, coordToUse.y - 1))
		{
			Tile tileToCheck = GetTile(coordToUse.x, coordToUse.y - 1);
			float valueToAdd = 1;
			lookAtValue = heatMap.GetHeat(tileToCheck.m_tileCoords);
			if (!tileToCheck.m_tileDef->m_allowsWalking)
				valueToAdd = HEAT_MAP_IMPASSABLE_VALUE + 1;
			if (initValue + valueToAdd < lookAtValue)
			{
				heatMap.SetHeat(tileToCheck.m_tileCoords, initValue + valueToAdd);
				openList.push(tileToCheck.m_tileCoords);
			}
		}
		//left
		if (CanGetTile(coordToUse.x - 1, coordToUse.y))
		{
			Tile tileToCheck = GetTile(coordToUse.x - 1, coordToUse.y);
			float valueToAdd = 1;
			lookAtValue = heatMap.GetHeat(tileToCheck.m_tileCoords);
			if (!tileToCheck.m_tileDef->m_allowsWalking)
				valueToAdd = HEAT_MAP_IMPASSABLE_VALUE + 1;
			if (initValue + valueToAdd < lookAtValue)
			{
				heatMap.SetHeat(tileToCheck.m_tileCoords, initValue + valueToAdd);
				openList.push(tileToCheck.m_tileCoords);
			}
		}
		//right
		if (CanGetTile(coordToUse.x + 1, coordToUse.y))
		{
			Tile tileToCheck = GetTile(coordToUse.x + 1, coordToUse.y);
			float valueToAdd = 1;
			lookAtValue = heatMap.GetHeat(tileToCheck.m_tileCoords);
			if (!tileToCheck.m_tileDef->m_allowsWalking)
				valueToAdd = HEAT_MAP_IMPASSABLE_VALUE + 1;
			if (initValue + valueToAdd < lookAtValue)
			{
				heatMap.SetHeat(tileToCheck.m_tileCoords, initValue + valueToAdd);
				openList.push(tileToCheck.m_tileCoords);
			}
		}
	}
}
