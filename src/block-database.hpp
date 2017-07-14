#pragma once

#include "block.hpp"
#include "color.hpp"

class BlockDatabase {
public:
	bool isBlockVisible(const BlockId blockId) const;

	Color3 getBlockColor(const BlockId blockId) const;
};
