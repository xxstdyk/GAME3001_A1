#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "Renderer.h"

StartScene::StartScene() {
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw() {

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 190, 235, 233, 255);
	drawDisplayList();
}

void StartScene::update() {
	updateDisplayList();
}

void StartScene::clean() {
	removeAllChildren();
}

void StartScene::handleEvents() {
	EventManager::Instance().update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE)) {
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1)) {
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start() {
	const SDL_Color black = { 0, 0, 0, 255 };
	m_pTitleLabel = new Label("Assignment 1", "Consolas", 54, black, glm::vec2(400.0f, 140.0f));
	m_pTitleLabel->setParent(this);
	addChild(m_pTitleLabel);

	m_pNameLabel = new Label("Nick Oostdyk - 101335730", "Consolas", 32, black, glm::vec2(400.0f, 220.0f));
	m_pNameLabel->setParent(this);
	addChild(m_pNameLabel);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f);

	m_pStartButton->addEventListener(CLICK, [&]()-> void {
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});

	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void {
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void {
		m_pStartButton->setAlpha(255);
	});

	addChild(m_pStartButton);


}

