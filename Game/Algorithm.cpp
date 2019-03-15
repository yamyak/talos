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
	move.xNew = 0;
	move.yNew = 0;
	move.xOld = 0;
	move.yOld = 0;

	return move;
}