#pragma once

#include <vector>
#include "Common.h"

class Algorithm
{
public:
	Algorithm();
	~Algorithm();

	Common::MoveRequest CalculateMove(Common::MiniBoard & board);
};

