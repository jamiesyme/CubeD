#include "block-database.hpp"

bool BlockDatabase::isBlockVisible(const BlockId blockId) const
{
	switch (blockId) {
	case BlockId::Void:
	case BlockId::Empty:
		return false;
	default:
		return true;
	}
}

Color3 BlockDatabase::getBlockColor(const BlockId blockId) const
{
	switch (blockId) {
	case BlockId::Solid:
		return {91, 209, 58};
	default:
		return (Color3)Color4::Black;
	}
}
