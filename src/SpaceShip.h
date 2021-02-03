#pragma once
#ifndef __SPACE_SHIP__
#define __SPACE_SHIP__
#include "DisplayObject.h"
#include "TextureManager.h"

enum class Behaviour { SEEK, FLEE, ARRIVE, AVOID, NUM_BEHAVIOURS };

class SpaceShip final : public DisplayObject {

	// Member Variables
	private:
	Behaviour m_currBehaviour;


	glm::vec2
		m_shipCenterPoint,
		m_destination,
		m_targetDirection,
		m_orientation;

	float
		m_maxSpeed,
		m_rotationAngle,
		m_turnRate,
		m_accelerationRate;

	// Member variables for debugging

	glm::vec2 m_whiskerEnds[2];
	float m_whiskerLen;

	int m_dangerRadius;

	bool
		m_drawWhiskers,
		m_drawDangerRadius;

	// Member Functions
	private:
	void LoadSounds();

	void LookWhereYoureGoing();
	void Move();

	void Seek();
	void Flee();
	void Arrive();
	void Avoid();

	// Public Variables
	public:
	DisplayObject *obstacle;

	// Public Functions
	public:
	// constructor(s)
	SpaceShip();

	// destructor
	~SpaceShip();

	// life-cycle methods inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	// Getters and setters
	Behaviour getCurrBehaviour() const;
	void setBehaviour(const Behaviour);

	void setDestination(const glm::vec2);
	void setMaxSpeed(const float);

	glm::vec2 getOrientation() const;
	void setOrientation(const glm::vec2);

	float getRotation() const;
	void setRotation(const float);

	float getTurnRate() const;
	void setTurnRate(const float);

	float getAccelerationRate() const;
	void setAccelerationRate(const float);

	void RenderWhiskers(bool);
	void RenderDangerRadius(bool);

};

#endif /* defined (__SPACE_SHIP__) */
