#pragma once

#include "Game/Map.hpp"
#include "Game/Camera.hpp"
#include "Game/Player.hpp"
#include "Game/Adventure.hpp"
#include "Game/SliderBar.hpp"
#include "Engine/Math/MathUtils.hpp"

enum GAME_STATE
{
	NONE,
	ATTRACT,
	PLAYING,
	PAUSED,
	DEFEAT,
	VICTORY,
	INVENTORY
};

class Game
{
public:
	~Game();
	Game();
	
	void Update();
	void Render();

	Map* GetCurrentMap();
	GAME_STATE GetCurrentState();
	void StartTransitionToState(GAME_STATE newGameState);

protected:
	void InitGame();
	void ClearGame();

	void Update_ATTRACT(float deltaSeconds);
	void Update_PLAYING(float deltaSeconds);
	void Update_PAUSED(float deltaSeconds);
	void Update_DEFEAT(float deltaSeconds);
	void Update_VICTORY(float deltaSeconds);
	void Update_INVENTORY(float deltaSeconds);

	void Render_ATTRACT();
	void Render_PLAYING();
	void Render_PAUSED();
	void Render_DEFEAT();
	void Render_VICTORY();
	void Render_INVENTORY();

	void RenderBlackOverlay();
	void StartFadeInBlackOverlay();
	void StartFadeOutBlackOverlay();

	void RenderHUD();

public:
	bool DEBUG_RENDER = false;
	Camera* m_camera = nullptr;
	Adventure* m_currentAdventure = nullptr;
	Player* m_player = nullptr;
	
private:
	float m_lastTime = 0;
	
	GAME_STATE m_currentState = GAME_STATE::ATTRACT;
	GAME_STATE m_transitionToState = GAME_STATE::NONE;  
	bool m_isFinishedTransitioning = true;
	float m_timeInCurrentState = 0;
	float m_timeToExitState = 0;
	float m_timeToStopTransitionIn = 0;
	bool m_fading = false;
	unsigned char m_blackOverlayAlpha = 0;

	const float FADE_IN_TIME = 0.85f;
	const float FADE_OUT_TIME = 0.85f;

	//HUD
	SliderBar m_healthBar;
	SliderBar m_staminaBar;
	Item* m_healthPotion;
	Texture* m_itemBorderTexture;
	Texture* m_arrowSwapTexture;
	Texture* m_arrowDownTexture;
};
