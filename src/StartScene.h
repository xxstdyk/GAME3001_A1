#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Scene.h"
#include "Label.h"
#include "ship.h"
#include "Button.h"

class StartScene final : public Scene {

	private:
	Label *m_pTitleLabel { };
	Label *m_pNameLabel { };

	Button *m_pStartButton;

	public:
	StartScene();
	~StartScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

};

#endif /* defined (__START_SCENE__) */