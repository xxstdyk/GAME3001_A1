#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__

#include "DisplayObject.h"

class Obstacle : public DisplayObject {

	public:

	// Constructors
	Obstacle();

	// Destructor
	~Obstacle();

	// Life cycle functions

	void draw() override;
	void update() override;
	void clean() override;
};

#endif /* defined (__OBSTACLE__) */