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

	//CollisionManager::AABBCheck(m_pSpaceShip, m_pObstacle);
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

	// Load Music
	SoundManager::Instance().load("../Assets/audio/bgMusic.mp3", "bgMusic", SoundType::SOUND_MUSIC);

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
}

void PlayScene::GUI_Function() const {
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("GAME3001 - Assignment 1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);


	if (ImGui::Button("1. Seeking")) {

	}

	ImGui::SameLine();

	if (ImGui::Button("2. Fleeing")) {

	}

	ImGui::SameLine();

	if (ImGui::Button("3. Arrival")) {

	}

	if (ImGui::Button("4. Obstacle Avoidance")) {

	}

	ImGui::Separator();

	static bool whiskersEnabled = false;
	if (ImGui::Checkbox("Enable Whiskers", &whiskersEnabled)) { 
	

	}

	ImGui::Separator();

	if (ImGui::Button("Start")) {

	}

	ImGui::SameLine();

	if (ImGui::Button("Reset")) {

	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
