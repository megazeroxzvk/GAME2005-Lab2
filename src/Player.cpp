#include "Player.h"
#include "TextureManager.h"
#include "Util.h"

Player::Player()
{
	TextureManager::Instance()->load("../Assets/textures/circle.png", "circle");
	
	auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Player::update()
{
	const float deltaTime = 1.0f / 60.f;

	//Lab 2: Normalize the vector for correct diagonal speed/acc
	//first magnitude is required.
	float directionMagnitude = Util::magnitude(m_direction);

	//If at all you get 0 as direction magnitude, it leads to a divide by 0
	//hence the check is required!
	if (directionMagnitude > 0)
	{
		getRigidBody()->acceleration = Util::normalize(m_direction) * ACCELERATION;
	}
	else if(Util::magnitude(getRigidBody()->velocity) > 0)	//check for velocity is 0 since that makes everything 0
	{
		//Lab2: A better deceleration with no shakiness
		if (Util::magnitude(getRigidBody()->velocity) < ACCELERATION)
		{
			getRigidBody()->velocity = { 0,0 };
			getRigidBody()->acceleration = { 0,0 };
			/*std::cout << "velocity x after setting to 0 =" << getRigidBody()->velocity.x << std::endl;*/
		}
		else
		{
			//braking over
			getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * -ACCELERATION;
			/*std::cout << "Normalvel.x =" << Util::normalize(getRigidBody()->velocity).x << std::endl;
			std::cout << "Normalvel.y =" << Util::normalize(getRigidBody()->velocity).y << std::endl;*/
		}
	}


	
	//velocity is always updated with the current acceleration
	getRigidBody()->velocity += getRigidBody()->acceleration;		//deltatime is corrected below.
	

	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;

	getTransform()->position = pos;
	
}

void Player::clean()
{

}

//Lab2 - fix for the diagonal faster movement.
//Key presses will set the direction of where to move
//SPEED is a factor which is already defined and set
//Accordingly multiply it with a 'normalized' factor of direction, and boom! you have it figured out.
void Player::moveLeft()
{
	m_direction.x = -1;
}

void Player::moveRight()
{
	m_direction.x = 1;
}

void Player::moveUp()
{
	m_direction.y = -1;
}							
							
void Player::moveDown()
{	
	m_direction.y = 1;
}

void Player::stopMovingX()
{
	m_direction.x = 0;
}

void Player::stopMovingY()
{
	m_direction.y = 0;
}

bool Player::isColliding(GameObject* pOther) {
	// Works for square sprites only
	float myRadius = getWidth() * 0.5f;
	float otherRadius = pOther->getWidth() * 0.5f;

	return (getDistance(pOther) <= myRadius + otherRadius);
}

float Player::getDistance(GameObject* pOther) {
	glm::vec2 myPos = getTransform()->position;
	glm::vec2 otherPos = pOther->getTransform()->position;

	// Use pythagorean to calculate distance c = sqrt(a^2 + b^2)
	float a = myPos.x - otherPos.x;
	float b = myPos.y - otherPos.y;
	return sqrt(a * a + b * b);
}
