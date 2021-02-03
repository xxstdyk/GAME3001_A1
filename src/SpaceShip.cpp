#include "SpaceShip.h"

#include "Util.h"
#include "Game.h"

SpaceShip::SpaceShip() {
	TextureManager::Instance()->load("../Assets/textures/ship.png", "spaceship");

	LoadSounds();

	auto size = TextureManager::Instance()->getTextureSize("spaceship");
	setWidth(size.x);
	setHeight(size.y);

	//setMaxSpeed(1.0f);
	setMaxSpeed(8.0f);
	setAccelerationRate(10.0f);
	setTurnRate(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	m_whiskerLen = 120.0f;
	m_dangerRadius = 150.0f;

	setType(SPACE_SHIP);
}

void SpaceShip::LoadSounds() {

	SoundManager::Instance().load("../Assets/audio/blip.mp3", "blip", SoundType::SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/crowdYay.mp3", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/ouch.mp3", "ouch", SoundType::SOUND_SFX);
}

SpaceShip::~SpaceShip()
= default;

void SpaceShip::draw() {

	TextureManager::Instance()->
		draw("spaceship", getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255);

	if (m_drawWhiskers)
		for (int i = 0; i < 2; ++i) Util::DrawLine(m_shipCenterPoint, m_whiskerEnds[i], glm::vec4(255, 255, 0, 0));

	if (m_drawDangerRadius)
		Util::DrawCircle(m_shipCenterPoint, m_dangerRadius, glm::vec4(255, 255, 0, 0));

}

void SpaceShip::update() {

	m_shipCenterPoint = glm::vec2(
		getTransform()->position.x + getWidth() / 2,
		getTransform()->position.y + getHeight() / 2);

	// Set whisker endpoints
	m_whiskerEnds[0] = m_shipCenterPoint + Util::getOrientation(m_rotationAngle - 30) * m_whiskerLen;
	m_whiskerEnds[1] = m_shipCenterPoint + Util::getOrientation(m_rotationAngle + 30) * m_whiskerLen;

	switch (m_currBehaviour) {
		case Behaviour::SEEK:	Seek();		break;
		case Behaviour::FLEE:	Flee();		break;
		case Behaviour::ARRIVE:	Arrive();	break;
		case Behaviour::AVOID:	Avoid();	break;
	}

	LookWhereYoureGoing();
	Move();
}

void SpaceShip::clean() { }

Behaviour SpaceShip::getCurrBehaviour() const {

	return m_currBehaviour;
}

void SpaceShip::setBehaviour(const Behaviour _newBehaviour) {

	m_currBehaviour = _newBehaviour;
}

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

void SpaceShip::RenderWhiskers(bool _val) {

	m_drawWhiskers = _val;
}

void SpaceShip::RenderDangerRadius(bool _val) {

	m_drawDangerRadius = _val;
}


void SpaceShip::LookWhereYoureGoing() {

	// Get the angle between current heading and line between ship and dest
	auto targetRotation = Util::signedAngle(getOrientation(), m_targetDirection);
	auto turnSensitivity = 5.0f;

	if (abs(targetRotation) > turnSensitivity) {
		if (targetRotation > 0.0f)
			setRotation(getRotation() + getTurnRate());
		else if (targetRotation < 0.0f)
			setRotation(getRotation() - getTurnRate());
	}
}

void SpaceShip::Move() {

	auto deltaTime = TheGame::Instance()->getDeltaTime();

	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	getRigidBody()->velocity += getOrientation() * deltaTime +
		0.5f * getRigidBody()->acceleration * deltaTime;

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}


// The simplest behaviours only just set the target direction
void SpaceShip::Seek() {

	// Direction with magnitude
	m_targetDirection = m_destination - m_shipCenterPoint;

	// Normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);
}


void SpaceShip::Flee() {

	// Direction with magnitude
	m_targetDirection = m_destination - m_shipCenterPoint;

	// Normalized direction
	m_targetDirection = Util::normalize(-m_targetDirection);

}

void SpaceShip::Arrive() {

	// Normalized direction
	m_targetDirection = Util::normalize(m_destination - m_shipCenterPoint);
	auto distToTarg = Util::distance(m_shipCenterPoint, m_destination);
	std::cout << distToTarg << std::endl;

	if (distToTarg < m_dangerRadius)
		getRigidBody()->velocity *= 0.95;
	
	if (distToTarg < 5.0f) { 
		setMaxSpeed(0);
	}
}

void SpaceShip::Avoid() {


}
