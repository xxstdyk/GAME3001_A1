#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene() {
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw() {

	if (EventManager::Instance().isIMGUIActive()) {
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 190, 235, 233, 255);
}

void PlayScene::update() {

	updateDisplayList();

	CollisionManager::AABBCheck(m_pSpaceShip, m_pObstacle);
}

void PlayScene::clean() {
	removeAllChildren();
}

void PlayScene::handleEvents() {
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE)) {
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1)) {
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2)) {
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start() {
	const SDL_Color black = { 0, 0, 0, 255 };

	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pHotkeyExplainerLabel = new Label("Use ` to open the IMGUI interface", "Consolas", 24, black, glm::vec2(400.0f, 24.0f));
	m_pHotkeyExplainerLabel->setParent(this);
	addChild(m_pHotkeyExplainerLabel);

	m_pInstructionsLabel = new Label("Use IMGUI to switch between behaviours", "Consolas", 24, black, glm::vec2(400.0f, 56.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);
	addChild(m_pTarget);

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(500.0f, 300.0f);
	addChild(m_pObstacle);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
	m_pSpaceShip->setEnabled(false);
	m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	addChild(m_pSpaceShip);
}

void PlayScene::GUI_Function() const {
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("GAME3001 - Assignment 1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	ImGui::Separator();

	if (ImGui::Button("Start")) {

		m_pSpaceShip->setEnabled(true);
	}	
	
	ImGui::SameLine();

	if (ImGui::Button("Reset")) {

		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 100.f);
		m_pSpaceShip->setEnabled(false);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
