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
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
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
	// Set GUI Title
	m_guiTitle = "Play Scene";

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

	ImGui::Begin("GAME3001 - Lab 2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float shipSpeed = 10.0f;
	if (ImGui::SliderFloat("MaxSpeed", &shipSpeed, 0.0f, 80.0f)) { 
	
		m_pSpaceShip->setMaxSpeed(shipSpeed);
	}

	static float accelerationRate = 2;

	if (ImGui::SliderFloat("Acceleration Rate", &accelerationRate, 0.0f, 50.0f)) { 
	
		m_pSpaceShip->setAccelerationRate(accelerationRate);
	}

	static float angInRadians = m_pSpaceShip->getRotation();
	if (ImGui::SliderAngle("Angle", &angInRadians)) {
		
		m_pSpaceShip->setRotation(angInRadians * Util::Rad2Deg);
	}

	static float turnRate = 5.0f;
	if (ImGui::SliderFloat("Turn Rate", &turnRate, 0.0f, 20.0f)) { 
	
		m_pSpaceShip->setTurnRate(turnRate);
	}

	if (ImGui::Button("Start")) {

		m_pSpaceShip->setEnabled(true);
	}	
	
	ImGui::SameLine();

	if (ImGui::Button("Reset")) {

		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 100.f);
		m_pSpaceShip->setEnabled(false);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f);

		turnRate = 5.0f;
		accelerationRate = 2.0f;
		shipSpeed = 10.0f;
		angInRadians = m_pSpaceShip->getRotation();
	}

	ImGui::Separator();

	static float targetPos[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if (ImGui::SliderFloat2("Target", targetPos, 0.0f, 800.0f)) {

		m_pTarget->getTransform()->position = glm::vec2(targetPos[0], targetPos[1]);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
