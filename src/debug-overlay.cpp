#include <iomanip>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "chunk-mesh-system.hpp"
#include "debug-overlay.hpp"
#include "player.hpp"
#include "world.hpp"

namespace {
	std::string toDebugStr(float num, int decimalPrecision) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(decimalPrecision);
		ss << num;
		return ss.str();
	}
}

DebugOverlay::DebugOverlay(const World& world)
	: world(world),
	  font(new sf::Font())
{
	this->font->loadFromFile("overpass-mono-regular.otf");

	auto newText = [&]() -> std::unique_ptr<sf::Text> {
		auto text = std::make_unique<sf::Text>();
		text->setFont(*this->font);
		text->setCharacterSize(24);
		return std::move(text);
	};

	this->textLines.push_back(newText()); // Triangles
	this->textLines.push_back(newText()); // Position
}

DebugOverlay::~DebugOverlay() = default;

void DebugOverlay::render(sf::RenderWindow& window)
{
	// Set triangles text
	const uint32_t triCount = this->world.chunkMeshSystem.getTriangleCount();
	const std::string triCountStr = std::to_string(triCount);
	const std::string fullTriStr = std::string("Triangles: ") + triCountStr;
	this->textLines[(size_t)LineIndex::Triangles]->setString(fullTriStr);

	// Set position text
	const glm::vec3 pos = this->world.players[0]->position;
	const std::string posStr = (toDebugStr(pos.x, 1) + ", " +
	                            toDebugStr(pos.y, 1) + ", " +
	                            toDebugStr(pos.z, 1));
	const std::string fullPosStr = std::string("Position: ") + posStr;
	this->textLines[(size_t)LineIndex::Position]->setString(fullPosStr);

	// Render the text lines
	sf::Transform transform;
	for (const auto& textLine : this->textLines) {
		window.draw(*textLine, transform);
		const auto charSize = textLine->getCharacterSize();
		transform.translate(0, this->font->getLineSpacing(charSize));
	}
}
