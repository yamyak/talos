#include "stdafx.h"
#include "Algorithm.h"

Algorithm::Algorithm()
{
}

Algorithm::~Algorithm()
{
}

Common::MoveRequest Algorithm::CalculateMove(std::vector<Common::PieceInfo> & mine, std::vector<Common::PieceInfo> & theirs)
{
	Common::MoveRequest move;
	move.newLoc.x = 0;
	move.newLoc.y = 0;
	move.oldLoc.x = 0;
	move.oldLoc.y = 0;

	return move;
}