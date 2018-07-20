#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"
#include "Game/ActorDefinition.hpp"
#include "Game/ItemDefinition.hpp"
#include "Game/Item.hpp"
#include "Game/TileDefinition.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntRange.hpp"
#include "Engine/Core/XmlUtilities.hpp"
#include "Engine/ThirdParty/tinyxml2.h"
#include "Engine/Core/EngineCommon.hpp"

Game::~Game()
{
	delete m_camera;
	delete g_terrainSpriteSheet;
	ClearGame();
}

Game::Game()
{
	InitRandomSeed();
	g_mainFont = g_theRenderer->CreateOrGetBitmapFont("SquirrelFixedFont");
	g_terrainSpriteSheet = new SpriteSheet(*g_theRenderer->CreateOrGetTexture("Data/Images/Terrain_8x8.png"), 8, 8);
	g_xboxController_1 = &g_theInput->GetController(0);

	TileDefinition::LoadDefinitions("Data/Definitions/Tiles.xml");
	ItemDefinition::LoadItemDefinitions("Data/Definitions/Items.xml");
	ItemDefinition::LoadLootTable("Data/Definitions/Loot.xml");
	ActorDefinition::LoadActorDefinitions("Data/Definitions/Actors.xml");
	ProjectileDefinition::LoadProjectileDefinitions("Data/Definitions/Projectiles.xml");
	PortalDefinition::LoadPortalDefinitions("Data/Definitions/Portals.xml");
	MapDefinition::LoadDefinitions("Data/Definitions/Maps.xml");
	AdventureDefinition::LoadAdventureDefinitions("Data/Definitions/Adventures.xml");

	m_camera = new Camera(10);
	m_healthPotion = new Item("healthPotion", ItemDefinition::GetItemDefinition("HealthPotion"), nullptr);
	m_itemBorderTexture = g_theRenderer->CreateOrGetTexture("Data/Images/itemBorder.png");
	m_arrowSwapTexture = g_theRenderer->CreateOrGetTexture("Data/Images/arrowSwap.png");
	m_arrowDownTexture = g_theRenderer->CreateOrGetTexture("Data/Images/arrowDown.png");
}

void Game::Update()
{
	float currentTime = static_cast<float>(GetCurrentTimeSeconds());	
	float deltaSeconds = min(currentTime - m_lastTime, 0.05f);
	m_lastTime = currentTime;

	if (g_theInput->WasKeyJustPressed(KEY_CODE::W))
		StartTransitionToState(GAME_STATE::VICTORY);
	if (g_theInput->WasKeyJustPressed(KEY_CODE::L))
		StartTransitionToState(GAME_STATE::DEFEAT);
	if (g_theInput->WasKeyJustPressed(KEY_CODE::F1))
		DEBUG_RENDER = !DEBUG_RENDER;

	if (m_transitionToState != GAME_STATE::NONE && m_isFinishedTransitioning)
	{
		// Init or Clear Game
		if (m_transitionToState == GAME_STATE::ATTRACT) //going into attract
		{
			ClearGame();
			StartFadeOutBlackOverlay();
		}
		else if (m_currentState == GAME_STATE::ATTRACT) //going out of attract
		{
			InitGame();
			StartFadeOutBlackOverlay();
		}

		m_currentState = m_transitionToState;
		m_timeInCurrentState = 0;
		m_transitionToState = GAME_STATE::NONE;
	}
	else
	{
		m_timeInCurrentState += deltaSeconds;
	}

	switch (m_currentState)
	{
		case GAME_STATE::ATTRACT: Update_ATTRACT(deltaSeconds); break;
		case GAME_STATE::PLAYING: Update_PLAYING(deltaSeconds); break;
		case GAME_STATE::PAUSED: Update_PAUSED(deltaSeconds); break;
		case GAME_STATE::DEFEAT: Update_DEFEAT(deltaSeconds); break;
		case GAME_STATE::VICTORY: Update_VICTORY(deltaSeconds); break;
		case GAME_STATE::INVENTORY: Update_INVENTORY(deltaSeconds); break;
	}	

	//Update black overlay
	if (m_fading)
	{
		//fading in
		if (!m_isFinishedTransitioning)
		{
			if (m_timeInCurrentState <= m_timeToExitState)
				m_blackOverlayAlpha = (unsigned char) Interpolate(255, 0, (m_timeToExitState - m_timeInCurrentState) / FADE_IN_TIME);
			else
			{
				m_isFinishedTransitioning = true;
				m_fading = false;
			}
		}
		//fading out
		else
		{
			if (m_timeInCurrentState <= m_timeToStopTransitionIn)
				m_blackOverlayAlpha = (unsigned char) Interpolate(255, 0, m_timeInCurrentState / FADE_OUT_TIME);
			else
				m_fading = false;
		}
	}

	//Update Camera (SetOrtho)
	m_camera->Update();
}

void Game::Render()
{	
	switch (m_currentState)
	{
		case GAME_STATE::ATTRACT: Render_ATTRACT(); break;
		case GAME_STATE::PLAYING: Render_PLAYING(); break;
		case GAME_STATE::PAUSED: Render_PAUSED(); break;
		case GAME_STATE::DEFEAT: Render_DEFEAT(); break;
		case GAME_STATE::VICTORY: Render_VICTORY(); break;
		case GAME_STATE::INVENTORY: Render_INVENTORY(); break; 
	}
}

Map* Game::GetCurrentMap()
{
	if (m_currentAdventure == nullptr)
		return nullptr;

	return m_currentAdventure->m_currentMap;
}

GAME_STATE Game::GetCurrentState()
{
	return m_currentState;
}

void Game::StartTransitionToState(GAME_STATE newGameState)
{
	if (!m_isFinishedTransitioning || m_fading)
		return;

	m_isFinishedTransitioning = true;
	m_transitionToState = newGameState;

	//trigger fade in for these cases
	if (m_currentState == GAME_STATE::ATTRACT || newGameState == GAME_STATE::ATTRACT)
	{
		m_isFinishedTransitioning = false;
		StartFadeInBlackOverlay();
	}
}

void Game::InitGame()
{
	m_currentAdventure = new Adventure("Flower Quest", AdventureDefinition::GetAdventureDefinition("Flower"));
	m_player = new Player("player", ActorDefinition::GetActorDefinition("Player"));
	m_currentAdventure->SpawnPlayerInAdventure(m_player);
	m_camera->MoveCameraTo(m_player->m_position);

	//Init HUD
	m_healthBar = SliderBar(Vector2(-8.5f, -3.6f), AABB2(0, 0, 3.2f, 0.25f), Rgba(255, 220, 220, 80), Rgba::green);
	m_staminaBar = SliderBar(Vector2(-8.5f, -3.2f), AABB2(0, 0, 3.2f, 0.2f), Rgba(255, 255, 255, 80), Rgba::yellow);
}

void Game::ClearGame()
{
	delete m_player;
	m_player = nullptr;
	delete m_currentAdventure;
	m_currentAdventure = nullptr;
}

void Game::Update_ATTRACT(float deltaSeconds)
{
	UNUSED(deltaSeconds);

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::START) || g_theInput->WasKeyJustPressed(KEY_CODE::SPACE))
		StartTransitionToState(GAME_STATE::PLAYING);
}

void Game::Update_PLAYING(float deltaSeconds)
{
	m_currentAdventure->Update(deltaSeconds);
	m_currentAdventure->LateUpdate(deltaSeconds);
	m_camera->MoveCameraTo(m_player->m_position);

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::START))
		StartTransitionToState(GAME_STATE::PAUSED);
	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::BACK))
		StartTransitionToState(GAME_STATE::INVENTORY);
}

void Game::Update_PAUSED(float deltaSeconds)
{
	UNUSED(deltaSeconds);

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::START))
		StartTransitionToState(GAME_STATE::PLAYING);
	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::BACK))
		StartTransitionToState(GAME_STATE::ATTRACT);
}

void Game::Update_DEFEAT(float deltaSeconds)
{
	UNUSED(deltaSeconds);

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::BACK))
		StartTransitionToState(GAME_STATE::ATTRACT);
	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::START))
	{
		StartTransitionToState(GAME_STATE::PLAYING);
		m_player->ResetPlayer(m_player->m_position, m_currentAdventure->m_currentMap);
	}
}

void Game::Update_VICTORY(float deltaSeconds)
{
	UNUSED(deltaSeconds);

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::START))
		StartTransitionToState(GAME_STATE::ATTRACT);
}

void Game::Update_INVENTORY(float deltaSeconds)
{
	UNUSED(deltaSeconds);

	if (g_xboxController_1->WasButtonJustPressed(XBOX_BUTTON_CODE::BACK))
		StartTransitionToState(GAME_STATE::PLAYING);
}

void Game::Render_ATTRACT()
{
	g_theRenderer->DrawTextInBox2D(AABB2(m_camera->m_worldPos - Vector2(6, 6), m_camera->m_worldPos + Vector2(6, 6)), "Elemental Dungeon\n\nPress Start To Play", 1.f
		, Rgba::white, 1, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.5f, 0.5f));
	RenderBlackOverlay();
}

void Game::Render_PLAYING()
{
	m_currentAdventure->Render();
	RenderHUD();
	RenderBlackOverlay();
}

void Game::Render_PAUSED()
{
	m_currentAdventure->Render();
	RenderHUD();
	g_theRenderer->DrawTextInBox2D(AABB2(m_camera->m_worldPos - Vector2(6, 6), m_camera->m_worldPos + Vector2(6, 6)), "Paused\n\nPress Start To Resume\nBack To Return", 1.f
		, Rgba::white, 1, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.5f, 0.5f));
	RenderBlackOverlay();
}

void Game::Render_DEFEAT()
{
	m_currentAdventure->Render();
	RenderHUD();
	g_theRenderer->DrawTextInBox2D(AABB2(m_camera->m_worldPos - Vector2(6, 6), m_camera->m_worldPos + Vector2(6, 6)), "Defeat\n\nPress Back To Return\nStart to Respawn", 1.f
		, Rgba::white, 1, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.5f, 0.5f));
	RenderBlackOverlay();
}

void Game::Render_VICTORY()
{
	m_currentAdventure->Render();
	RenderHUD();
	g_theRenderer->DrawTextInBox2D(AABB2(m_camera->m_worldPos - Vector2(6, 6), m_camera->m_worldPos + Vector2(6, 6)), "Victory\n\nPress Start To Return", 1.f
		, Rgba::white, 1, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.5f, 0.5f));
	RenderBlackOverlay();
}

void Game::Render_INVENTORY()
{
	//Render player stats
	Stats currentStats = m_player->GetCurrentStats();
	std::string statsText = "HP: " + std::to_string(m_player->m_currentHealth) + "/" + std::to_string(m_player->m_actorDef->m_maxHealth) + "\n\n"
						+ "Fire ATK: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_FIRE_ATK]) + "\n" +
						+ "Water ATK: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_WATER_ATK]) + "\n" +
						+ "Earth ATK: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_EARTH_ATK]) + "\n\n" +
						+ "Fire DEF: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_FIRE_DEF]) + "\n" +
						+ "Water DEF: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_WATER_DEF]) + "\n" +
						+ "Earth DEF: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_EARTH_DEF]);

	std::string itemsText = "";
	Strings itemSlotName = Strings({ "Head: ", "Head ALT: ", "Chest: ", "Chest ALT: "});
	//hard coded to correspond to head, head alt, chest, chest alt
	for (int i = 1; i < 5; i++)
	{
		Item* item = m_player->m_equippedItems[i];
		itemsText = itemsText + itemSlotName[i - 1];
		if (item != nullptr)
		{
			itemsText += item->m_name + "\n\n";
			if (item->m_stats.m_statValues[STAT_ID::STAT_FIRE_ATK] != 0)
				itemsText += "Fire ATK: " + std::to_string(item->m_stats.m_statValues[STAT_ID::STAT_FIRE_ATK]) + "\n";
			if (item->m_stats.m_statValues[STAT_ID::STAT_WATER_ATK] != 0)
				itemsText += "Water ATK: " + std::to_string(item->m_stats.m_statValues[STAT_ID::STAT_WATER_ATK]) + "\n";
			if (item->m_stats.m_statValues[STAT_ID::STAT_EARTH_ATK] != 0)
				itemsText += "Earth ATK: " + std::to_string(item->m_stats.m_statValues[STAT_ID::STAT_EARTH_ATK]) + "\n";
			if (item->m_stats.m_statValues[STAT_ID::STAT_FIRE_DEF] != 0)
				itemsText += "Fire DEF: " + std::to_string(item->m_stats.m_statValues[STAT_ID::STAT_FIRE_DEF]) + "\n";
			if (item->m_stats.m_statValues[STAT_ID::STAT_WATER_DEF] != 0)
				itemsText += "Water DEF: " + std::to_string(item->m_stats.m_statValues[STAT_ID::STAT_WATER_DEF]) + "\n";
			if (item->m_stats.m_statValues[STAT_ID::STAT_EARTH_DEF] != 0)
				itemsText += "Earth DEF: " + std::to_string(item->m_stats.m_statValues[STAT_ID::STAT_EARTH_DEF]);
		}
		itemsText = itemsText + "\n\n";
	}

	std::string enemyText = "Targeted Enemy: ";
	if (m_player->m_itemTransferTarget != nullptr)
	{
		Actor* enemy = m_player->m_itemTransferTarget;
		enemyText += "\n\n" + enemy->m_name + "\n\n";
		currentStats = enemy->GetCurrentStats();
		Stats enemyStats = enemy->GetCurrentStats();
		std::string enemyStatsText = "HP: " + std::to_string(enemy->m_currentHealth) + "/" + std::to_string(enemy->m_actorDef->m_maxHealth) + "\n\n"
			+ "Fire ATK: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_FIRE_ATK]) + "\n" +
			+ "Water ATK: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_WATER_ATK]) + "\n" +
			+ "Earth ATK: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_EARTH_ATK]) + "\n\n" +
			+ "Fire DEF: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_FIRE_DEF]) + "\n" +
			+ "Water DEF: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_WATER_DEF]) + "\n" +
			+ "Earth DEF: " + std::to_string(currentStats.m_statValues[STAT_ID::STAT_EARTH_DEF]);
		enemyText += enemyStatsText;
	}

	AABB2 textBox = AABB2(0, 0, 5, 8);
	textBox.Translate(Vector2(m_camera->m_bounds.mins.x + 0.7f, m_camera->m_bounds.maxs.y - textBox.maxs.y - 1.f));
	g_theRenderer->DrawTextInBox2D(textBox, statsText, 0.25f, Rgba::white, 1, nullptr, TEXT_DRAW_MODE::OVERRUN, Vector2(0.f, 0.f));
	textBox.Translate(Vector2(textBox.GetDimensions().x + 0.7f, 0));
	g_theRenderer->DrawTextInBox2D(textBox, itemsText, 0.25f, Rgba::white, 1, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.f, 0.f));
	textBox.Translate(Vector2(textBox.GetDimensions().x + 0.7f, 0));
	g_theRenderer->DrawTextInBox2D(textBox, enemyText, 0.25f, Rgba::white, 1, nullptr, TEXT_DRAW_MODE::OVERRUN, Vector2(0.f, 0.f));

	RenderBlackOverlay();
}

void Game::RenderBlackOverlay()
{
	//TODO: figure out why SetOrtho is still weird
	//g_theRenderer->PushMatrix();
	//g_theRenderer->TranslateInMatrix(m_camera->m_worldPos);		
	//g_theRenderer->SetOrtho(Vector2(0, 0), Vector2(1, 1));
	g_theRenderer->DrawAABB(m_camera->m_bounds, Rgba(0, 0, 0, m_blackOverlayAlpha));
	//g_theRenderer->PopMatrix();
}

void Game::StartFadeInBlackOverlay()
{
	m_fading = true;
	m_timeToExitState = m_timeInCurrentState + FADE_IN_TIME;
}

void Game::StartFadeOutBlackOverlay()
{
	m_fading = true;
	m_timeToStopTransitionIn = FADE_OUT_TIME;
}

void Game::RenderHUD()
{
	m_healthBar.m_fillColor = m_player->m_healthBar.m_fillColor;
	m_healthBar.m_value = (float) m_player->m_currentHealth / m_player->m_actorDef->m_maxHealth;
	m_staminaBar.m_value = (float) m_player->m_currentStamina / m_player->MAX_STAMINA;
	m_healthBar.Render(m_camera->m_worldPos, Vector2(0, 0));
	m_staminaBar.Render(m_camera->m_worldPos, Vector2(0, 0));

	//Potion
	AABB2 UI_Box = AABB2(0, 0, 0.8f, 0.8f);
	UI_Box.Translate(m_camera->m_bounds.mins.x + 0.4f, m_camera->m_bounds.mins.y + 0.4f);
	AABB2 UvsToUse = m_healthPotion->m_spriteAnimSet->GetCurrentUVs();
	g_theRenderer->DrawTexturedAABB(UI_Box, m_healthPotion->m_spriteAnimSet->GetCurrentTexture(), UvsToUse.mins, UvsToUse.maxs, m_healthPotion->m_entityDef->m_defaultTint);
	g_theRenderer->DrawTextInBox2D(UI_Box, "x" + std::to_string(m_player->GetPotionCount()), 0.2f, Rgba::yellow, 1.f, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(1.f, 0));
	g_theRenderer->DrawTextInBox2D(UI_Box - Vector2(0, 0.25f), "LB", 0.2f, Rgba::white, 1.f, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.5f, 1.f));

	//Head
	AABB2 UI_Arrow_Box = AABB2(0, 0, 0.4f, 0.3f);
	UI_Arrow_Box.Translate(m_camera->m_bounds.mins.x + 1.8f, m_camera->m_bounds.mins.y + 0.1f);
	UI_Box.Translate(1.0f, 0);
	Item* itemToShow = m_player->m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD];
	if (itemToShow != nullptr)
	{
		UvsToUse = itemToShow->m_spriteAnimSet->GetCurrentUVs();
		g_theRenderer->DrawTexturedAABB(UI_Box, itemToShow->m_spriteAnimSet->GetCurrentTexture(), UvsToUse.mins, UvsToUse.maxs, itemToShow->m_entityDef->m_defaultTint);
	}
	else
	{
		g_theRenderer->DrawAABB(UI_Box, Rgba(0, 0, 0, 160));
	}
	if (m_player->m_selectedEquipSlot == EQUIP_SLOT::EQUIP_SLOT_HEAD)
	{
		g_theRenderer->DrawTexturedAABB(UI_Box, *m_itemBorderTexture, Vector2(0, 0), Vector2(1, 1), Rgba::yellow);
		if (m_player->m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_HEAD_ALT] != nullptr)
		{
			g_theRenderer->DrawTexturedAABB(UI_Arrow_Box, *m_arrowSwapTexture, Vector2(0, 0), Vector2(1, 1), Rgba::white);
		}
	}
	g_theRenderer->DrawTextInBox2D(UI_Box - Vector2(0, 0.25f), "Y", 0.2f, Rgba::white, 1.f, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.25f, 1.f));

	//Chest
	UI_Box.Translate(1.0f, 0);
	UI_Arrow_Box.Translate(1.0f, 0);
	itemToShow = m_player->m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST];
	if (itemToShow != nullptr)
	{
		UvsToUse = itemToShow->m_spriteAnimSet->GetCurrentUVs();
		g_theRenderer->DrawTexturedAABB(UI_Box, itemToShow->m_spriteAnimSet->GetCurrentTexture(), UvsToUse.mins, UvsToUse.maxs, itemToShow->m_entityDef->m_defaultTint);
	}
	else
	{
		g_theRenderer->DrawAABB(UI_Box, Rgba(0, 0, 0, 160));
	}
	if (m_player->m_selectedEquipSlot == EQUIP_SLOT::EQUIP_SLOT_CHEST)
	{
		g_theRenderer->DrawTexturedAABB(UI_Box, *m_itemBorderTexture, Vector2(0, 0), Vector2(1, 1), Rgba::yellow);
		if (m_player->m_equippedItems[EQUIP_SLOT::EQUIP_SLOT_CHEST_ALT] != nullptr)
		{
			g_theRenderer->DrawTexturedAABB(UI_Arrow_Box, *m_arrowSwapTexture, Vector2(0, 0), Vector2(1, 1), Rgba::white);
		}
	}
	g_theRenderer->DrawTextInBox2D(UI_Box - Vector2(0, 0.25f), "B", 0.2f, Rgba::white, 1.f, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.25f, 1.f));

	//Pick up hint
	if (m_player->m_itemToPickUp != nullptr)
	{
		AABB2 textBox = AABB2(0, 0, 3.f, 1.f);
		textBox.Translate(m_camera->m_worldPos.x - (textBox.maxs.x * 0.5f), m_camera->m_bounds.maxs.y - 1.3f);
		g_theRenderer->DrawTextInBox2D(textBox, "A to pick up", 0.5f, Rgba::white, 1.f, nullptr, TEXT_DRAW_MODE::SHRINK_TO_FIT, Vector2(0.5f, 0.5f));
	}

	//Transfer target
	Actor* target = m_player->m_itemTransferTarget;
	if (target != nullptr && m_player->m_mapAttached == target->m_mapAttached)
	{
		AABB2 box = AABB2(0, 0, 0.3f, 0.3f);	
		box.Translate(Vector2(target->m_position.x - (box.maxs.x * 0.5f), target->m_position.y + target->m_actorDef->m_localDrawBounds.maxs.y + 0.05f));
		g_theRenderer->DrawTexturedAABB(box, *m_arrowDownTexture, Vector2(0, 0), Vector2(1, 1), Rgba::white);
	}
}
