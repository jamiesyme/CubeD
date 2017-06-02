#include <algorithm>
#include <cmath>

#include "player.hpp"

Player::Player(World& world)
	: world(world),
	  rotationX(0.f),
	  rotationY(0.f)
{
}

void Player::rotateX(float deg)
{
	this->rotationX = std::clamp(this->rotationX + deg, -90.f, 90.f);
}

void Player::rotateY(float deg)
{
	this->rotationY = fmod(this->rotationY + deg, 360.f);
}

float Player::getRotationX() const
{
	return this->rotationX;
}

float Player::getRotationY() const
{
	return this->rotationY;
}
