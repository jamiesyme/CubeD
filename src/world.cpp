#include <SFML/OpenGL.hpp>

#include "player.hpp"
#include "world.hpp"

World::World() = default;

World::~World() = default;

void World::render()
{
	float s = 0.5f;
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-s, -s, 0.f);
	glVertex3f( s, -s, 0.f);
	glVertex3f( s,  s, 0.f);
	glVertex3f( s,  s, 0.f);
	glVertex3f(-s,  s, 0.f);
	glVertex3f(-s, -s, 0.f);
	glEnd();
}

Player& World::createPlayer()
{
	this->players.emplace_back(new Player(*this));
	return *this->players.back();
}
