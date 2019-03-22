#pragma once

#include <set>
#include "Common.h"
#include "Piece.h"

class Validator
{
public:
	Validator();
	~Validator();

	bool CheckMoveRequest(Common::MoveRequest & move, Common::Color color, Common::MiniBoard & board);
	bool CheckStraightPathForAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board, int xStep, int yStep,  std::set<Common::PieceType> types);
	bool CheckPawnAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board);
	bool CheckKnightAggressors(std::pair<int, int> kingLoc, Common::Color color, Common::MiniBoard & board);

private:
	bool CheckMovePath(Common::MoveRequest & move , Common::MiniBoard & board);
	bool CheckPathForObstacles(Common::MoveRequest & move, Common::MiniBoard & board);
};

