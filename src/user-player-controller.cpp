#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <SFML/Window.hpp>

#include "player.hpp"
#include "system-event.hpp"
#include "user-player-controller.hpp"

UserPlayerController::UserPlayerController(Player& player)
	: player(player),
	  lookSpeedX(0.1f),
	  lookSpeedY(0.1f),
	  moveSpeedX(5.0f),
	  moveSpeedY(5.0f),
	  moveSpeedZ(5.0f),
	  previousMouseX(sf::Mouse::getPosition().x),
	  previousMouseY(sf::Mouse::getPosition().y)
{
}

void UserPlayerController::onEvent(const SystemEvent& systemEvent)
{
	const sf::Event& sfEvent = systemEvent.sfEvent;

	switch (sfEvent.type) {
	case sf::Event::MouseMoved: {
		int deltaX = sfEvent.mouseMove.x - this->previousMouseX;
		int deltaY = sfEvent.mouseMove.y - this->previousMouseY;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			player.rotateX(-this->lookSpeedX * deltaY);
			player.rotateY(-this->lookSpeedY * deltaX);
		}

		this->previousMouseX = sfEvent.mouseMove.x;
		this->previousMouseY = sfEvent.mouseMove.y;
	} break;

	default:
		break;
	}
}

void UserPlayerController::tick(float deltaTime)
{
	glm::mat4 viewMat;
	viewMat = glm::rotate(viewMat,
	                      -glm::radians(player.getRotationX()),
	                      glm::vec3(1.f, 0.f, 0.f));
	viewMat = glm::rotate(viewMat,
	                      -glm::radians(player.getRotationY()),
	                      glm::vec3(0.f, 1.f, 0.f));

	glm::vec3 relMoveVec;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		relMoveVec.z -= this->moveSpeedZ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		relMoveVec.x -= this->moveSpeedX;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		relMoveVec.z += this->moveSpeedZ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		relMoveVec.x += this->moveSpeedX;
	}

	glm::vec3 absMoveVec = glm::vec3(glm::vec4(relMoveVec, 0) * viewMat);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		absMoveVec.y += this->moveSpeedY;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		absMoveVec.y -= this->moveSpeedY;
	}

	float boostMultiplier = 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		boostMultiplier = 3.0f;
	}

	player.position += absMoveVec * boostMultiplier * deltaTime;
}
