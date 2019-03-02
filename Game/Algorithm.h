#pragma once

#include <vector>
#include "Common.h"

class Algorithm
{
public:
	Algorithm();
	~Algorithm();

	Common::MoveRequest CalculateMove(std::vector<Common::PieceInfo> & mine, std::vector<Common::PieceInfo> & theirs);
};

