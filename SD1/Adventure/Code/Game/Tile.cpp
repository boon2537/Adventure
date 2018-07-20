#include "Game/Tile.hpp"

Tile::Tile(IntVector2 coords, TileDefinition * def)
	: m_tileCoords(coords)
	, m_tileDef(def)
{}

AABB2 Tile::GetWorldBounds(float tileWidth, float tileHeight)
{
	return AABB2(m_tileCoords.x * tileWidth, m_tileCoords.y * tileHeight
		, (m_tileCoords.x + 1) * tileWidth, (m_tileCoords.y + 1) * tileHeight);
}

void Tile::ChangeTileDef(TileDefinition* newTileDef)
{
	m_tileDef = newTileDef;
}
