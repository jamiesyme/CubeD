#pragma once

#include <memory>
#include <vector>

namespace sf {
	class Font;
	class RenderWindow;
	class Text;
}

class World;

class DebugOverlay {
public:
	DebugOverlay(const World& world);

	~DebugOverlay();

	void render(sf::RenderWindow& window);

private:
	enum class LineIndex {
		Triangles = 0,
		Position
	};
	const World& world;
	std::unique_ptr<sf::Font> font;
	std::vector<std::unique_ptr<sf::Text>> textLines;
};
