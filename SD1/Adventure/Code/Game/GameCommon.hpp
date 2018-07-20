#pragma once
#include "Game/Game.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Audio/AudioSystem.hpp"

extern Renderer* g_theRenderer;
extern InputSystem* g_theInput;
extern Game* g_theGame;
extern SpriteSheet* g_terrainSpriteSheet;
extern BitmapFont* g_mainFont;
extern AudioSystem* g_audio;
extern XboxController* g_xboxController_1;
