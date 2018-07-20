#pragma once

#include "Game/MapGenStep.hpp"
#include "Engine/Core/Image.hpp"

class MapGenStep_FromFile : public MapGenStep
{
public:
	~MapGenStep_FromFile();
	MapGenStep_FromFile( const tinyxml2::XMLElement& genStepXmlElement );
	virtual void Run( Map& map ) override;

public:
	Image* m_imageData = nullptr;
};
