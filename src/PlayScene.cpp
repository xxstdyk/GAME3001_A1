#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

#include "SoundManager.h"

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

	CollisionManager::AABBCheck(m_pSpaceShip, m_pTarget);

	updateDisplayList();

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
		m_pSpaceShip->setBehaviour(Behaviour::SEEK);
		SetupScene(m_pSpaceShip->getCurrBehaviour());
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2)) {
		m_pSpaceShip->setBehaviour(Behaviour::FLEE);
		SetupScene(m_pSpaceShip->getCurrBehaviour());
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3)) {
		m_pSpaceShip->setBehaviour(Behaviour::ARRIVE);
		SetupScene(m_pSpaceShip->getCurrBehaviour());
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4)) {
		m_pSpaceShip->setBehaviour(Behaviour::AVOID);
		SetupScene(m_pSpaceShip->getCurrBehaviour());
	}
}

void PlayScene::start() {


	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Load sounds
	SoundManager::Instance().load("../Assets/audio/bgMusic.mp3", "bgMusic", SoundType::SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/blip.wav", "blip", SoundType::SOUND_SFX);
	SoundManager::Instance().setSoundVolume(32);

	// Play Music
	SoundManager::Instance().setMusicVolume(48);
	SoundManager::Instance().playMusic("bgMusic");

	// Labels
	const SDL_Color black = { 0, 0, 0, 255 };

	m_pHotkeyExplainerLabel = new Label("Use ` to open the IMGUI interface", "Consolas", 24, black, glm::vec2(400.0f, 24.0f));
	m_pHotkeyExplainerLabel->setParent(this);
	addChild(m_pHotkeyExplainerLabel);

	m_pInstructionsLabel = new Label("Use IMGUI or 1-4 to switch between behaviours", "Consolas", 24, black, glm::vec2(400.0f, 56.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	// Target
	m_pTarget = new Target();
	m_pTarget->setEnabled(true);
	addChild(m_pTarget);

	// Obstacle
	m_pObstacle = new Obstacle();
	m_pObstacle->setEnabled(false);
	m_pObstacle->getTransform()->position = glm::vec2(
		400 - m_pObstacle->getWidth()/2, 300 - m_pObstacle->getHeight() / 2
	);
	addChild(m_pObstacle);

	// Ship
	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->setEnabled(true);
	m_pSpaceShip->setDestination(glm::vec2(
		m_pTarget->getTransform()->position.x + m_pTarget->getWidth() / 2,
		m_pTarget->getTransform()->position.y + m_pTarget->getHeight() / 2));
	m_pSpaceShip->obstacle = m_pObstacle;
	addChild(m_pSpaceShip);
}


// Code to handle the moving and resetting of all the object positions
void PlayScene::SetupScene(const Behaviour _behaviour) {

	SoundManager::Instance().playSound("blip");

	switch (_behaviour) {

		case Behaviour::SEEK:
		case Behaviour::ARRIVE:

			m_pObstacle->setEnabled(false);

			m_pTarget->getTransform()->position = glm::vec2(480, 280);

			m_pSpaceShip->setDestination(glm::vec2(
				m_pTarget->getTransform()->position.x + m_pTarget->getWidth() / 2,
				m_pTarget->getTransform()->position.y + m_pTarget->getHeight() / 2));
			m_pSpaceShip->getTransform()->position = glm::vec2(-100, 100);
			m_pSpaceShip->setMaxSpeed(8.0f);

			break;

		case Behaviour::FLEE:

			m_pObstacle->setEnabled(false);

			m_pTarget->getTransform()->position = glm::vec2(480, 280);

			m_pSpaceShip->setDestination(glm::vec2(
				m_pTarget->getTransform()->position.x + m_pTarget->getWidth() / 2,
				m_pTarget->getTransform()->position.y + m_pTarget->getHeight() / 2));
			m_pSpaceShip->getTransform()->position = glm::vec2(300 - 64, 200 - 64);
			m_pSpaceShip->setMaxSpeed(8.0f);

			break;

		case Behaviour::AVOID:

			m_pTarget->getTransform()->position = glm::vec2(680, 280);

			m_pSpaceShip->setDestination(glm::vec2(
				m_pTarget->getTransform()->position.x + m_pTarget->getWidth() / 2,
				m_pTarget->getTransform()->position.y + m_pTarget->getHeight() / 2));
			m_pSpaceShip->getTransform()->position = glm::vec2(-100, 100);
			m_pSpaceShip->setMaxSpeed(8.0f);

			m_pObstacle->setEnabled(true);

			break;

	}

}

void PlayScene::GUI_Function() {
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("GAME3001 - Assignment 1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);


	if (ImGui::Button("1. Seeking")) {

		m_pSpaceShip->setBehaviour(Behaviour::SEEK);
		SetupScene(m_pSpaceShip->getCurrBehaviour());

	}

	ImGui::SameLine();

	if (ImGui::Button("2. Fleeing")) {

		m_pSpaceShip->setBehaviour(Behaviour::FLEE);
		SetupScene(m_pSpaceShip->getCurrBehaviour());

	}

	ImGui::SameLine();

	if (ImGui::Button("3. Arrival")) {

		m_pSpaceShip->setBehaviour(Behaviour::ARRIVE);
		SetupScene(m_pSpaceShip->getCurrBehaviour());

	}

	ImGui::SameLine();

	if (ImGui::Button("4. Avoidance")) {

		m_pSpaceShip->setBehaviour(Behaviour::AVOID);
		SetupScene(m_pSpaceShip->getCurrBehaviour());

	}

	ImGui::Separator();

	static bool whiskersEnabled = false;
	if (ImGui::Checkbox("Show Whiskers", &whiskersEnabled)) {

		m_pSpaceShip->RenderWhiskers(whiskersEnabled);
	}

	static bool dangerRadiusEnabled = false;
	if (ImGui::Checkbox("Show Slowdown Radius", &dangerRadiusEnabled)) {

		m_pSpaceShip->RenderDangerRadius(dangerRadiusEnabled);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
