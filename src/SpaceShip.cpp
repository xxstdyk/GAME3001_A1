#include "SpaceShip.h"

#include "Util.h"
#include "Game.h"

SpaceShip::SpaceShip() {
	TextureManager::Instance()->load("../Assets/textures/spaceship.png", "spaceship");

	auto size = TextureManager::Instance()->getTextureSize("spaceship");
	setWidth(size.x);
	setHeight(size.y);

	setMaxSpeed(10.0f);
	setAccelerationRate(10.0f);
	setTurnRate(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SPACE_SHIP);
}

SpaceShip::~SpaceShip()
= default;

void SpaceShip::draw() {

	TextureManager::Instance()->
		draw("spaceship", getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	Util::DrawLine(getTransform()->position, getTransform()->position + m_orientation * 60.0f);
}

void SpaceShip::update() {

	m_Move();
}

void SpaceShip::clean() { }

void SpaceShip::setDestination(const glm::vec2 _dest) {

	m_destination = _dest;
}

void SpaceShip::setMaxSpeed(const float _speed) {

	m_maxSpeed = _speed;
}

glm::vec2 SpaceShip::getOrientation() const {
	return m_orientation;
}

void SpaceShip::setOrientation(const glm::vec2 _orientation) {

	m_orientation = _orientation;
}

float SpaceShip::getRotation() const {
	return m_rotationAngle;
}

void SpaceShip::setRotation(const float _angle) {

	m_rotationAngle = _angle;
	auto angInRads = (_angle - 90.0f) * Util::Deg2Rad;

	auto x = cos(angInRads);
	auto y = sin(angInRads);

	// convert ang to normalized vec and store in orientation
	setOrientation(glm::vec2(x, y));
}


float SpaceShip::getTurnRate() const {
	return m_turnRate;
}

void SpaceShip::setTurnRate(const float _turnRate) {
	m_turnRate = _turnRate;
}

float SpaceShip::getAccelerationRate() const {
	return m_accelerationRate;
}

void SpaceShip::setAccelerationRate(const float _accelerationRate) {
	m_accelerationRate = _accelerationRate;
}


void SpaceShip::m_Move() {

	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// Direction with magnitude
	m_targetDirection = m_destination - getTransform()->position;

	// Normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto targetRotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turnSensitivity = 5.0f;

	if (abs(targetRotation) > turnSensitivity) { 
	
		if (targetRotation > 0.0f) {

			setRotation(getRotation() + getTurnRate());
		} else if (targetRotation < 0.0f) {

			setRotation(getRotation() - getTurnRate());
		}
	}



	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	getRigidBody()->velocity += getOrientation() * deltaTime +
		0.5f * getRigidBody()->acceleration * deltaTime;

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}
