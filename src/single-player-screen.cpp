#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Time.hpp>

#include "game.hpp"
#include "player.hpp"
#include "single-player-screen.hpp"
#include "user-player-controller.hpp"
#include "world.hpp"

SinglePlayerScreen::SinglePlayerScreen(GameSettings& gameSettings, World& world)
	: gameSettings(gameSettings),
	  world(world),
	  userPlayerController(new UserPlayerController(world.createPlayer()))
{
	this->userPlayerController->player.position = glm::vec3(0.f, 0.f, -10.f);
	this->userPlayerController->player.rotateY(180);
}

SinglePlayerScreen::~SinglePlayerScreen() = default;

void SinglePlayerScreen::onEvent(const SystemEvent& systemEvent)
{
	this->userPlayerController->onEvent(systemEvent);
}

void SinglePlayerScreen::tick(float deltaTime)
{
	this->userPlayerController->tick(deltaTime);
}

void SinglePlayerScreen::render(sf::Window& window)
{
	// Get camera settings
	Player& player = this->userPlayerController->player;
	float cameraNear = 0.1f;
	float cameraFar = 100.f;
	float cameraAspect = (float)window.getSize().x / window.getSize().y;
	float cameraFov = glm::radians(this->gameSettings.fov);
	float cameraRotX = glm::radians(player.getRotationX());
	float cameraRotY = glm::radians(player.getRotationY());
	glm::vec3 cameraPos = player.position;

	// Setup opengl matrices
	glm::mat4 projMat = glm::perspective(cameraFov,
	                                     cameraAspect,
	                                     cameraNear,
	                                     cameraFar);
	glm::mat4 viewMat;
	viewMat = glm::rotate(viewMat, -cameraRotX, glm::vec3(1.f, 0.f, 0.f));
	viewMat = glm::rotate(viewMat, -cameraRotY, glm::vec3(0.f, 1.f, 0.f));
	viewMat = glm::translate(viewMat, -cameraPos);
	glLoadIdentity();
	glMultMatrixf(&(projMat * viewMat)[0][0]);

	// Setup depth buffer and backface culling
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Render the world
	this->world.render();
}
