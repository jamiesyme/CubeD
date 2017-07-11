#pragma once

enum class BlockId {
	Void = 0,
	Empty,
	Solid
};

namespace Block {
	static inline float getSize()
	{
		return 0.5f;
	}
}
