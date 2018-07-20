#pragma once

#include "Game/MapDefinition.hpp"
#include "Game/Tile.hpp"
#include "Game/Entity.hpp"
#include "Game/Actor.hpp"
#include "Game/Portal.hpp"
#include "Game/Projectile.hpp"
#include "Game/Player.hpp"
#include "Game/Item.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Core/HeatMap.hpp"
#include <vector>
#include <string>

typedef std::vector<Actor*> ActorList;
typedef std::vector<Projectile*> ProjectileList;
typedef std::vector<Portal*> PortalList; 
typedef std::vector<Item*> ItemList;

struct RayCastResult2D
{
	RayCastResult2D()
	{
		// default for non-hit result
		m_didImpact = false;
		m_impactPos = Vector2(9999, 9999);
		m_impactTileCoords = IntVector2(9999, 9999);
		m_impactDistance = 0;
		m_impactFraction = 100;
		m_impactSurfaceNormal = Vector2(0, 0);
	}

	RayCastResult2D(bool didImpact, Vector2 impactPos, IntVector2 impactTileCoord, float impactDistance, float impactFraction, Vector2 impactSurfaceNormal)
		: m_didImpact(didImpact)
		, m_impactPos(impactPos)
		, m_impactTileCoords(impactTileCoord)
		, m_impactDistance(impactDistance)
		, m_impactFraction(impactFraction)
		, m_impactSurfaceNormal(impactSurfaceNormal)
	{}

	bool m_didImpact;
	Vector2 m_impactPos;
	IntVector2 m_impactTileCoords;
	float m_impactDistance;
	float m_impactFraction;
	Vector2 m_impactSurfaceNormal;
};

class Map
{
public:
	~Map() {};
	Map(const std::string& name, MapDefinition* def);

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Render();

	bool CanGetTile(int x, int y);
	Tile& GetTile(int x, int y);
	Tile& GetClosetTileFromPosition(const Vector2& position);
	Tile* GetRandomTileOfDef(const TileDefinition* def);
	std::vector<Tile*> GetTilesOfDef(const TileDefinition* def);
	Vector2 GetClosestPointToTile(const Vector2 &center, Tile &tile);
	bool CanEntityEnterTile(const Tile tile, const Entity* entity);
	
	Projectile* SpawnProjectile(const std::string& typeName, Actor* shooter, const Vector2& pos, float orientation);
	Actor* SpawnActor(const std::string& typeName, const Vector2& pos, float orientation);
	Portal* SpawnPortal(const std::string& typeName, const Vector2& pos, float orientation);
	Item* SpawnItem(const std::string& typeName, const Vector2& pos, float orientation);
	void ReceiveItem(Item* item, const Vector2& pos, float orientation);
	void RemovePickedUpItem(Item* item);
	void AttachPlayerToMap(Player* player);
	void DeattachPlayerFromMap(Player* player);

	bool HasLineOfSight( const Vector2& startPos, const Vector2& endPos ); //TODO: make this const later
	RayCastResult2D RayCast(const Vector2& start, const Vector2& direction, float distance);

	void UpdateDistanceField(HeatMap& heatMap, const std::vector<IntVector2>& seedCoords);

public:
	std::string m_name;
	int m_mapHeight;
	int m_mapWidth;
	MapDefinition* m_mapDefinition;
	std::vector<Tile> m_tiles;

	std::vector<Entity*> m_entities;
	ActorList m_actors;
	ProjectileList m_projectiles;
	PortalList m_portals;
	ItemList m_itemsOnGround;
	std::vector<IntVector2> m_playerCoord;
	HeatMap m_playerDistanceField;

	const int RAYCAST_STEPS_PER_TILE = 1000;
	const float HEAT_MAP_FILL_VALUE = 1000000;
	const float HEAT_MAP_IMPASSABLE_VALUE = 2000;
};
