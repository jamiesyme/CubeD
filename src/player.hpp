#pragma once

#include <glm/vec3.hpp>

class World;

class Player {
public:
	Player(World& world);

	// Rotate on the x-axis. Clamps between -90 and 90.
	void rotateX(float deg);

	// Rotate on the y-axis. Wraps between -360 and 360.
	void rotateY(float deg);

	// Get the rotation on the x-axis in degrees.
	float getRotationX() const;

	// Get the rotation on the y-axis in degrees.
	float getRotationY() const;

	World& world;
	glm::vec3 position;

private:
	float rotationX;
	float rotationY;
};
