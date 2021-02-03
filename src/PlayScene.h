#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "SpaceShip.h"
#include "Target.h"
#include "Obstacle.h"

class PlayScene : public Scene {

	private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;

	SpaceShip *m_pSpaceShip;
	Target *m_pTarget;
	Obstacle *m_pObstacle;

	// UI Items
	Label *m_pHotkeyExplainerLabel { };
	Label *m_pInstructionsLabel { };

	// Private funcs
	private:
	void SetupScene(const Behaviour);

	// Public funcs
	public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;


};

#endif /* defined (__PLAY_SCENE__) */