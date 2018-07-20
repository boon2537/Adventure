#pragma once

#include "Game/TileDefinition.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/AABB2.hpp"

class Tile
{
public:
	~Tile() {};
	Tile(IntVector2 coords, TileDefinition* def);

	AABB2 GetWorldBounds(float tileWidth = 1.f, float tileHeight = 1.f);
	void ChangeTileDef(TileDefinition* newTileDef);

public:
	IntVector2 m_tileCoords;
	TileDefinition* m_tileDef = nullptr;
};